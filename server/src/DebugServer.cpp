#include "DebugServer.h"


using json = nlohmann::json;
using String = std::string;

std::deque<Message> g_ReadQueue;
std::deque<Message> g_WriteQueue;

std::mutex g_ReadQueueMutex;
std::mutex g_WriteQueueMutex;

// loop lock �����Ѿ���� ��������thread��asio ʵ��thread��readQ writeQ�Ĺ�ϵ
int g_MsgIdGen = 0;

//DebugServer* g_Server = nullptr;


Message::Message(std::string msg, int type)
{
	this->content = msg;
	this->id = g_MsgIdGen++;
	this->type = type;
}

void Message::log()
{
	std::cout << "msg id:" << id << std::endl
		<< " type:" << type << std::endl
		<< " content:" << content << std::endl;
}

String Message::wrapMsg(String type, json message)
{
	message["type"] = type;
	message["seq"] = g_MsgIdGen++;
	String msg = message.dump();
	std::cout << "readyToSend:" << std::endl
		<< msg << std::endl;

	int len = msg.length();
	String wrapped("");
	wrapped = wrapped + "Content-Length: " + std::to_string(len)
		+ "\r\n\r\n" + msg;
	return wrapped;
}


void MessageDispatcher::Dispatch(lua_State* L,Message _msg, MessageHandler* handler)
{
	//call(msg);
	std::cout << "Dispatch : " << std::endl;
	_msg.log();
	//handler->Send(_msg);
	String msg = _msg.content;
	json request = json::parse(msg);
	json newResponse = json::parse("{}");
	newResponse["request_seq"] = request["seq"];
	newResponse["command"] = request["command"];
	newResponse["success"] = true;
	auto type = request["type"];
	if (type == "request")
	{
		String command = request["command"];
		std::cout << command << std::endl;
		if (command == "initialize") {
			newResponse["body"] =
			{
				{ "supportsConfigurationDoneRequest",true },
				{ "supportsEvaluateForHovers",true },
				{ "supportsStepBack",true }
			};
			handler->SendResponse(newResponse);
			
			json initMsg = json::parse("{}");
			initMsg["event"] = "initialized";
			handler->SendEvent(initMsg);

			lua_getglobal(L, "SetTrapWait");
			lua_pushboolean (L, true);
			lua_call(L, 1, 0);
		}
		else if (command == "launch") {
			/*
			start runtime
			start server to listen runtime
			runtime should call mobdebug.start to connect runtime server
			when runtime server is received start command
			*/
			handler->SendResponse(newResponse);
			json _event = json::parse("{}");
			_event["event"] = "stopOnEntry";
			handler->SendEvent(_event);


		}
		else if (command == "attach") {

		}
		else if (command == "disconnect") {
			lua_getglobal(L, "SetTrapWait");
			lua_pushboolean(L, false);
			lua_call(L, 1, 0);
		}
		else if (command == "restart") {

		}
		else if (command == "setBreakpoints") {
			//server->doWrite(wrapMsg(newResponse,"reso"));
		}
		else if (command == "setFunctionBreakpoints") {

		}
		else if (command == "setExceptionBreakpoints") {
			//server->doWrite(wrapMsg(newResponse));
		}
		else if (command == "configurationDone") {

		}
		else if (command == "continue") {
			lua_getglobal(L, "SetTrapWait");
			lua_pushboolean(L, false);
			lua_call(L, 1, 0);
		}
		else if (command == "next") {

		}
		else if (command == "stepIn") {

		}
		else if (command == "stepOut") {

		}
		else if (command == "stepBack") {

		}
		else if (command == "reverseContinue") {

		}
		else if (command == "restartFrame") {

		}
		else if (command == "goto") {

		}
		else if (command == "pause") {
			lua_getglobal(L, "SetTrapWait");
			lua_pushboolean(L, false);
			lua_call(L, 1, 0);
		}
		else if (command == "stackTrace") {

		}
		else if (command == "scopes") {

		}
		else if (command == "variables") {

		}
		else if (command == "setVariable") {

		}
		else if (command == "source") {

		}
		else if (command == "threads") {

		}
		else if (command == "evaluate") {

		}
		else if (command == "stepInTargets") {

		}
		else if (command == "gotoTargets") {

		}
		else if (command == "completions") {

		}
		else if (command == "exceptionInfo") {

		}
		else if (command == "loadedSources") {

		}
		else {

		}

	}
	else if (type == "response")
	{

	}
}


MessageHandler::MessageHandler(DebugServer* server) 
:m_Server(server)
{
	m_Dispatcher = new MessageDispatcher();
}

MessageHandler::~MessageHandler()
{
	delete m_Dispatcher;
}

void MessageHandler::Loop(lua_State*L)
{
	if (!g_ReadQueue.empty() || !g_WriteQueue.empty())
	{
		if (!g_ReadQueue.empty())
		{
			g_ReadQueueMutex.lock();
			auto msg = g_ReadQueue.front();
			g_ReadQueue.pop_front();
			g_ReadQueueMutex.unlock();
			m_Dispatcher->Dispatch(L,msg, this);
		}
		if (!g_WriteQueue.empty())
		{
			g_WriteQueueMutex.lock();
			auto msg = g_WriteQueue.front();
			g_WriteQueue.pop_front();
			g_WriteQueueMutex.unlock();
			m_Server->Write(msg);
		}
	}
}

void MessageHandler::Send(Message msg)
{
	g_WriteQueueMutex.lock();
	g_WriteQueue.push_back(msg);
	g_WriteQueueMutex.unlock();
}

bool MessageHandler::SendEvent(json& msgjson)
{
	Message msg("");
	msg.content = msg.wrapMsg("event",msgjson);
	Send(msg);
	return true;
}

bool MessageHandler::SendResponse(json& msgjson)
{
	Message msg("");
	msg.content = msg.wrapMsg("response", msgjson);
	Send(msg);
	return true;
}

bool MessageHandler::SendRequest(json& msgjson)
{
	Message msg("");
	msg.content = msg.wrapMsg("request", msgjson);
	Send(msg);
	return true;
}



DebugSession::DebugSession(int id,asio::ip::tcp::socket& socket)
{
	m_ID = id;
	m_Socket = new asio::ip::tcp::socket(std::move(socket));
	mReadBuffCurrentIndex = 0;
	mReadBuff.resize(4096);
	//m_Handler = new MessageHandler();
}

DebugSession::~DebugSession()
{
//	delete m_Handler;
}

void DebugSession::DoRead()
{
	if (m_Socket->is_open())
	{
		asio::async_read(*m_Socket, asio::buffer(m_OneByte, 1),
			[this](std::error_code ec, std::size_t len)
		{
			if (!ec)
			{
				mReadBuff[mReadBuffCurrentIndex++] = m_OneByte[0];
				std::string s(mReadBuff.begin(), mReadBuff.begin() + mReadBuffCurrentIndex);
				std::regex regex_match_txt("Content-Length: ([0-9]+)\r\n\r\n");
				std::smatch base_match;
				if (std::regex_match(s, base_match, regex_match_txt))
				{
					if (base_match.size() == 2)
					{
						std::ssub_match lenstr = base_match[1];
						int len = std::stoi(lenstr.str());
						asio::async_read(*m_Socket, asio::buffer(m_MsgBuff, len),
							[this](std::error_code ec, std::size_t len)
						{
							if (!ec)
							{
								mReadBuffCurrentIndex = 0;
								for (int i = 0; i < len; i++)
								{
									mReadBuff[mReadBuffCurrentIndex++] = m_MsgBuff[i];
								}
								auto msgstr = std::string(mReadBuff.begin(), mReadBuff.begin() + mReadBuffCurrentIndex);
								mReadBuffCurrentIndex = 0;
								EnReadQueue(msgstr);
							}
							else
							{
								std::cout << ec.message() << std::endl;
							}
						});
					}
				}
				DoRead();
			}
			else
			{
				std::cout << ec.message() << std::endl;
				m_Socket->close();
				std::cout << "Socket is close"<< std::endl;
			}

		});
	}
}

void DebugSession::DoReadRunable()
{
	DoRead();
}

void DebugSession::EnReadQueue(const std::string& msgstr)
{

	g_ReadQueueMutex.lock();
	g_ReadQueue.push_back(Message(msgstr));
	g_ReadQueueMutex.unlock();
}

void DebugSession::DoWriteRunable()
{
	
}

void DebugSession::Write(Message msg)
{
    std::cout << " DebugSession::Write(Message msg)" << std::endl;
	msg.log();
	if (m_Socket->is_open()) {
		asio::async_write(*m_Socket,
			asio::buffer(msg.content.c_str(),
				msg.content.length()),
			[this](std::error_code ec, std::size_t /*length*/)
		{

		});
	}
}

void DebugSession::Start()
{
	std::cout << "Session start!" << std::endl;
	//std::thread readThread(std::bind(&DebugSession::DoReadRunable, this));
	
	DoReadRunable();
	
	DoWriteRunable();
	
	//std::thread writeThread(std::bind(&DebugSession::DoWriteRunable, this));
}


DebugServer::DebugServer(int port)
{	
	m_Port = port;
	m_Session = nullptr;
	m_SessionIdGen = 0;
	asio::ip::tcp::endpoint localhost(asio::ip::tcp::v4(), port);
	m_Acceptor = new asio::ip::tcp::acceptor(m_IOContext, localhost);
//	m_Handler = new MessageHandler();
}

DebugServer::~DebugServer()
{
	delete m_Acceptor;
}

void DebugServer::RunSession()
{
//	std::thread(std::bind(&DebugServer::RunLooper, this));
	//RunSession(socket);
	
	// auto& socket_ptr = m_Sockets[m_SessionIdGen];
	// DebugSession* session = new DebugSession(m_SessionIdGen, *socket_ptr);
	// m_Sockets.erase(m_SessionIdGen);

	// std::cout << " new connection:" << m_SessionIdGen << std::endl;

	// m_SessionIdGen++;

	// session->Start();
	//m_Sessions.insert(std::make_pair(m_SessionIdGen, session));		
	
//	auto& sessionIter = m_Sessions.find(m_SessionIdGen);
//	sessionIter->second->Start();
	

}

void DebugServer::Listen()
{
	std::cout << " server Listen! port:" << m_Port << std::endl;
	m_Acceptor->async_accept(
		[this](std::error_code ec, asio::ip::tcp::socket socket)
	{
		if (!ec)
		{
			m_Session = new DebugSession(m_SessionIdGen, socket);
		//	session_mutext.lock();
			// m_Sessions.insert(std::make_pair(m_SessionIdGen, session));
		//	session_mutext.unlock();
			m_Session->Start();
			//std::thread(std::bind(&DebugServer::RunSession, this));
		}
		else 
		{
			std::cout << "m_Acceptor->async_accept error : " << ec.message() << std::endl;
		} 

		//session->Start();
//		Listen();		
	});

}

void DebugServer::Run()
{
	m_IOContext.run();
}

void DebugServer::Write(Message msg)
{
	std::cout << "server send post" << std::endl;
	asio::post(m_IOContext,
        [this, msg]()
        {
			std::cout << "server send" << std::endl;
			m_Session->Write(msg);
        });
}


