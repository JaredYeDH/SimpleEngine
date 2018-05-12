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
#include "DebuggerCore.h"
#include <functional>
#include "nlohmann\json.hpp"


int port = 4712;
MessageHandler* handler;
DebugServer* server;
void RunLooper()
{
	server = new DebugServer(port);
	handler = new  MessageHandler(server);
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


static int lua_run_loop(lua_State* L)
{
	handler->Loop(L);
	return 0;
}
#if 1
int main(int argc, char* argv[])
{
	try
	{
		std::thread t(RunLooper);

		lua_State *L;
		L = luaL_newstate();
		luaL_openlibs(L);

		lua_register(L, "lua_sleep", lua_sleep);
		lua_register(L, "lua_run_loop", lua_run_loop);

		//std::string(WORK_DIR) + "/scripts/main.lua"
		std::string main_script_path = std::string("D:\\Github\\vscode-mock-debug\\src\\tests\\main.lua").c_str();
		std::cout << main_script_path << std::endl;;
		int ec = luaL_dofile(L, main_script_path.c_str());
		if (ec) {
			std::cout << "luaL_dofile error!" << std::endl;
		}

		DebuggerCore* instance = DebuggerCore::GetInstance(); //dbgCore(L, handler);
															  /*lua_getglobal(L, "SetHook");
															  lua_call(L, 0, 0);
															  */
		instance->SetHandler(handler);
		instance->SetHook(L);

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
#else
std::vector<Json> m_Sources;
std::map<String, Json> m_AllBreakPoints;

void VerifyAllBreakPoints()
{
	int id = 1;
	for (auto& pb_pair : m_AllBreakPoints)
	{
		auto& bp_ary = pb_pair.second;
		for (Json::iterator bp_it = bp_ary.begin(); bp_it != bp_ary.end(); ++bp_it)
		{
			bp_it.value()["id"] = id++;
			bp_it.value()["verified"] = true;
		}
	}
	std::cout << m_AllBreakPoints << std::endl;
}

void SetAllBreakPoints(String path, Json bps)
{
	std::cout << bps << std::endl;
	assert(bps.is_array());
	m_AllBreakPoints[path] = Json::array();
	Json& new_bps = m_AllBreakPoints[path];
	for (Json::iterator bp_it = bps.begin(); bp_it != bps.end(); ++bp_it)
	{
		Json new_bp = Json::object();
		new_bp["id"] = 0;
		new_bp["verified"] = false;
		new_bp["line"] = bp_it.value()["line"];
		new_bps.push_back(new_bp);
	}
}

int main()
{
	Json js = Json::parse(R"*({"command":"setBreakpoints","arguments":{"source":{"name":"testLazyBreakpoint.md","path":"D:\\Github\\vscode-mock-debug\\src\\tests\\data\\testLazyBreakpoint.md"},"lines":[1,2,4],"breakpoints":[{"line":1},{"line":2},{"line":4}],"sourceModified":false},"type":"request","seq":4})*");
	Json arg = js["arguments"];
	Json bps = arg["breakpoints"];
	//std::cout << bps << std::endl;
	SetAllBreakPoints("asd", bps);
	VerifyAllBreakPoints();

	String path(R"(F:\game\pub\data\core\modules\map\map_data.lua)");
	std::replace(path.begin(), path.end(), '\\', '/');

	std::cout << path << std::endl;
	/*for (Json::iterator it = js.begin(); it != js.end(); ++it) {

	std::cout << it.key() << " : " << it.value() << std::endl;
	auto bpt = it.value();
	if (bpt.is_array())
	{
	for (Json::iterator it2 = bpt.begin(); it2 != bpt.end(); ++it2) {
	std::cout <<  it2.value() << std::endl;

	for (Json::iterator it3 = it2.value().begin(); it3 != it2.value().end(); ++it3) {
	std::cout << it3.key() << " : " << it3.value() << std::endl;

	}

	}
	}

	}*/

	int bp;

	//system("pause");
	return 0;
}
#endif


