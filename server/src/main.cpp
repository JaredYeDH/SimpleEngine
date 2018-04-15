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


DebugServer* server;

void RunLooper()
{
	// MessageHandler* handler = new  MessageHandler();
	// handler->Loop();


    server->Listen();
    server->Run();
}

int main(int argc, char* argv[])
{
	try
	{

         server = new DebugServer(port);
		 
        std::thread t(RunLooper);

        MessageHandler* handler = new  MessageHandler(server);
        handler->Loop();

	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
