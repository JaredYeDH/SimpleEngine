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


int port = 4712;
DebugServer* server;
void RunLooper()
{
    server->Listen();
    server->Run();
}


void SleepMs(long ms)
{
#ifdef WIN32
	Sleep(ms);
#else
	sleep(ms);
#endif
}

void Lua_OnUpdate(lua_State* L)
{
	lua_getglobal(L, "OnGameUpdate");
	lua_call(L, 0, 0);
}
void printGetTop(lua_State* L)
{
	std::cout << "lua_gettop:" <<  lua_gettop(L) << std::endl;
}

static int lua_sleep(lua_State* L)
{
	std::cout << "lua_sleep" << std::endl;
	int ms = luaL_checknumber(L, 1);
	SleepMs(ms);
	return 0;
}

MessageHandler* handler;
static int lua_run_loop(lua_State* L)
{
	handler->Loop(L);
	return 0;
}

int main(int argc, char* argv[])
{
	try
	{
		
		server = new DebugServer(port);
        std::thread t(RunLooper);


		handler = new  MessageHandler(server);
		lua_State *L;
		L = luaL_newstate();
		luaL_openlibs(L);

		lua_register(L, "lua_sleep", lua_sleep);
		lua_register(L, "lua_run_loop", lua_run_loop);
		
		std::string main_script_path = (std::string(WORK_DIR) + "/scripts/main.lua").c_str();
		std::cout << main_script_path << std::endl;;
		int ec = luaL_dofile(L, main_script_path.c_str());
		if (ec) {
			std::cout << "luaL_dofile error!" << std::endl;
		}

		lua_getglobal(L, "SetHook");
		lua_call(L, 0, 0);


		while (true)
		{
			handler->Loop(L);
			Lua_OnUpdate(L);


			
			SleepMs(1000.0f / 60);
		}
        
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
