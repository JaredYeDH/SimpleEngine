#include "defines.h"
extern "C"{
#include <luasocket.h>
}
int main()
{
	lua_State *L ;
	L= luaL_newstate();
	
	luaL_openlibs(L);
    luaopen_socket_core(L);
	luaL_dofile(L, "/Users/oceancx/MHXY/SimpleEngine/scripts/server/main.lua");

	return 0;
}
