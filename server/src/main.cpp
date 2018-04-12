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
	int port = 4711;
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

	
	//try
	//{
	//	// Check command line arguments.
	//	if (argc != 4)
	//	{
	//		std::cerr << "Usage: http_server <address> <port> <doc_root>\n";
	//		std::cerr << "  For IPv4, try:\n";
	//		std::cerr << "    receiver 0.0.0.0 80 .\n";
	//		std::cerr << "  For IPv6, try:\n";
	//		std::cerr << "    receiver 0::0 80 .\n";
	//		return 1;
	//	}

	//	// Initialise the server.
	//	http::server::server s(argv[1], argv[2], argv[3]);

	//	// Run the server until stopped.
	//	s.run();
	//}
	//catch (std::exception& e)
	//{
	//	std::cerr << "exception: " << e.what() << "\n";
	//}
	return 0;
}
