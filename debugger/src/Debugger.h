#pragma once
#include "defines.h"

using String = std::string;

class DebugSession;

class Dispatcher
{
public:
    void handleMessage(DebugSession* session, String msg);

};


class DebugSession
{
public:
	DebugSession();

	DebugSession(int port);
	
	~DebugSession();

	void read();
	
	void write();

	void run();

private:
	asio::io_context m_IOContext;
	asio::ip::tcp::socket* m_Socket;
	int  m_Port;

	std::deque<std::string> mMsgQueue;
	Dispatcher m_Dispatcher;
};


