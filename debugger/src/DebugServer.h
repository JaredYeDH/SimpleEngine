#pragma once
#include "defines.h"

class DebugServer;

struct Message
{
	int id;
	int type;
	std::string content;
	Message() {};
	Message(std::string msg, int type = 0);
	
	String wrapMsg(String type, nlohmann::json message);
	void log();
	
};

class MessageHandler;
class DebugSession;

class MessageDispatcher
{
public:
	void Dispatch(lua_State* L,Message msg, MessageHandler* handler);
	std::function<void()> onConfidoneFunc;
};

class MessageHandler
{
public:
	MessageHandler(DebugServer* server);

	~MessageHandler();

	void Loop(lua_State*L);

	bool SendEvent(nlohmann::json& msg);

	bool SendResponse(nlohmann::json& msg);

	bool SendRequest(nlohmann::json& msg);

	void Send(Message msg);
	void StopOnBreakPointEvent(Json bp);

private:
	DebugServer* m_Server;
	MessageDispatcher* m_Dispatcher;
	
	


};

class DebugSession
{
public:
	DebugSession(int id, asio::ip::tcp::socket& socket);
	
	~DebugSession();

	void DoReadRunable();
	void DoRead();
	void EnReadQueue(const std::string& msgstr);
	void DoWriteRunable();
	void Write(Message msg);
	void Start();

private:
	asio::ip::tcp::socket* m_Socket;
	std::vector<char> mReadBuff;
	int mReadBuffCurrentIndex;
	char m_MsgBuff[4096];
	char m_OneByte[1];
	int m_ID;
	bool m_IsReading;
};

class DebugServer
{
public:
	DebugServer(int port);
	
	~DebugServer();
	
	void Listen();

	void Run();

	void RunSession();

	void Write(Message msg);


private:
	DebugSession* m_Session;
	asio::ip::tcp::acceptor* m_Acceptor;
	asio::io_context m_IOContext;
//	MessageHandler* m_Handler;
	int m_SessionIdGen = 0;
	int m_Port;
};



//
//class RuntimeSession
//{
//public:
//	RuntimeSession();
//	~RuntimeSession();
//
//private:
//
//};
//
//RuntimeSession::RuntimeSession()
//{
//
//}
//
//RuntimeSession::~RuntimeSession()
//{
//
//}


#define  RAWBUFF_LENGTH 1024
class RawBuff
{
public:

	RawBuff()
		:m_Buffsize(RAWBUFF_LENGTH)
	{
	};

	~RawBuff()
	{

	};
	
	char* buff(){ return m_Buff; };

	int buffsize(){ return m_Buffsize; };

	void print(){ std::cout <<std::string(m_Data)<< std::endl; };

	void clear() { m_ContentSize = 0; };

	void appendBuf() { 
		strncpy(&m_Data[m_ContentSize], m_Buff, m_Buffsize); 
		m_ContentSize += m_Buffsize; 
		
	};

	int size() { return m_ContentSize; };
private:
	char m_Buff[RAWBUFF_LENGTH];
	int m_Buffsize;
	int m_ContentSize;
	char m_Data[4096];
};



