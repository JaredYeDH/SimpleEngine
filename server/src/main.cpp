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



void runLua()
{
	lua_State *L;
	L = luaL_newstate();
	luaL_openlibs(L);

	luaL_dofile(L, "D:/Github/mobdebug/examples/server.lua");
	std::cout << "block me" << std::endl;
	std::cout << "server end" << std::endl;
}

int port = 4712;

void RunServer()
{	

	g_Server->Run();
}

void RunLooper()
{
	MessageHandler* handler = new  MessageHandler();
	handler->Loop();

	
}

int main(int argc, char* argv[])
{
	try
	{
		asio::io_context m_IOContext;
		
		std::map<int, DebugSession*> m_Sessions;
		g_Server = new DebugServer(port, m_IOContext, m_Sessions);
		g_Server->Listen();
		
		std::thread t([&m_IOContext]() { m_IOContext.run(); });
		
		RunLooper();
		//std::thread t(runLua);
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
