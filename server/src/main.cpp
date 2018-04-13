#include "defines.h"

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include "asio.hpp"
#include "DebugServer.h"
#include "server.hpp"
#include <mutex>
#include <thread>
// loop lock 架子已经搭好 坐等整合thread跟asio 实现thread跟readQ writeQ的关系
struct Message
{

};


std::deque<Message> g_ReadQueue;
std::deque<Message> g_WriteQueue;

std::mutex g_ReadQueueMutex;
std::mutex g_WriteQueueMutex;

class MessageDispatcher
{
public:
	void Dispatch(const Message& msg) 
	{
		//call(msg);
	}
};

class MessageHandler
{
public:
	MessageHandler()
	{}

	~MessageHandler()
	{}

	void Loop()
	{
		while (!g_ReadQueue.empty())
		{
			g_ReadQueueMutex.lock();
			auto& msg = g_ReadQueue.front();
			g_ReadQueue.pop_front();
			g_ReadQueueMutex.unlock();
			m_Dispatcher.Dispatch(msg);
		}
	}

	void Send(const Message& msg)
	{
		g_WriteQueueMutex.lock();
		g_WriteQueue.push_back(msg);
		g_WriteQueueMutex.unlock();
	}

private:
	MessageDispatcher m_Dispatcher;
};

class DbgSession 
{
public:
	DbgSession()
	{
		DoReadRunable();		//new Thread1
		DoWriteRunable();		//new Thread2
	}

	~DbgSession()
	{

	}

	void DoReadRunable()
	{
		Message msg;
		std::cin >> msg;
		g_ReadQueueMutex.lock();
		g_ReadQueue.push_back(msg);
		g_ReadQueueMutex.unlock();
	}

	void DoWriteRunable()
	{
		while (!g_WriteQueue.empty())
		{
			g_WriteQueueMutex.lock();
			auto& msg = g_WriteQueue.front();
			g_WriteQueue.pop_front();
			g_WriteQueueMutex.unlock();
			Write(msg);
		}
	}

	void Write(Message msg)
	{
		std::cout << msg;
	}
	void Read(Message msg)
	{
		std::cin << msg;
	}

}

class DbgServer 
{
public:
	DbgServer(int port)
	{
		m_IdGen = 0;
	}
	~DbgServer()
	{
		
	}
	void Listen()
	{
		DbgSession session;
		m_Sessions[m_IdGen++] = &session;
	}


private:
	std::map<int,DbgSession*> m_Sessions;
	int m_IdGen;
}

void runLua()
{
	lua_State *L;
	L = luaL_newstate();
	luaL_openlibs(L);

	luaL_dofile(L, "D:/Github/mobdebug/examples/server.lua");
	std::cout << "block me" << std::endl;
	std::cout << "server end" << std::endl;
}

int main(int argc, char* argv[])
{



	int port = 4712;
	try
	{
		DebugServer server(port);
		server.run();
		
		//std::thread t(runLua);
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
