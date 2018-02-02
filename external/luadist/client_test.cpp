#include <iostream>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
//#include <luasocket.h>
}


using namespace std;
int main()
{
    lua_State *L ;
    L= luaL_newstate();
    luaL_openlibs(L);
  //  luaopen_socket_core(L);
    luaL_dofile(L, "/Users/oceancx/luadist/bin/hello.lua" );
    return 0;
}
