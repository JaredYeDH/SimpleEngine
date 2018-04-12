#include "DebugServer.h"
using std::cout;
using std::endl;

DebugSession::DebugSession()
{
}

DebugSession::~DebugSession()
{
}



void Dispatcher::handleMessage(DebugServer* server, String msg)
{
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
			server->sendResponse(newResponse);
			json initMsg = json::parse("{}");
			initMsg["event"] = "initialized";
			server->sendEvent(initMsg);
		}
		else if (command == "launch") {
			/*
			start runtime
			start server to listen runtime
			runtime should call mobdebug.start to connect runtime server
			when runtime server is received start command
			*/   
			server->sendResponse(newResponse);
			json _event = json::parse("{}");
			_event["event"] = "stopOnEntry";
			server->sendEvent(_event);
		}
		else if (command == "attach") {

		}
		else if (command == "disconnect") {

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

DebugServer::DebugServer(int port)
{
	_sequence = 1;
	asio::ip::tcp::endpoint localhost(asio::ip::tcp::v4(), port);
	m_Acceptor = new asio::ip::tcp::acceptor(m_IOContext, localhost);
	mReadBuffCurrentIndex = 0;
	mReadBuff.resize(4096);
}

DebugServer::~DebugServer()
{

}

void DebugServer::run()
{
	std::thread t(std::bind(&DebugServer::backThread, this));
	while (true) {
		if (!mMsgQueue.empty())
		{
			std::string msg = mMsgQueue.front();
			mMsgQueue.pop_front();

			std::cout <<"popMsg:" <<endl<< msg << std::endl;
			m_Dispatcher.handleMessage(this, msg);

		}
		else
		{
#if WIN32
			Sleep(1);
#else
			sleep(1);
#endif
		}
	}
}

bool DebugServer::sendEvent( json& msg)
{
	doWrite(wrapMsg("event", msg));
	return true;
}

bool DebugServer::sendResponse( json& msg)
{
	doWrite(wrapMsg("response", msg));
	return true;
}

bool DebugServer::sendRequest(json& msg)
{
	doWrite(wrapMsg("request", msg));
	return true;
}
