#pragma once
#include "defines.h"
#include "defineTypes.h"
#include "Singleton.h"
#include "Environment.h" 


class LuaVM : public Singleton<LuaVM>
{
public:
    friend Singleton<LuaVM>;
    static lua_State* get(){return L;};
    void Init()
    {
        L = luaL_newstate();
        luaL_openlibs(L);
        int error = luaL_dofile(L,Environment::GetLuaPath("main.lua").c_str());
        if(error)
        {
            std::cout <<" load dofile error!" << std::endl;
            return ;
        }
    }
    void Close()
    {
	    lua_close(L);
    }
    
    void doLuaString(std::string luastring)
    {
        luaL_dostring(L,luastring.c_str());
    }
    //  c++ call lua demo
    //  lua_getglobal(L, "OnGameInit");                  /* function to be called */
    //  lua_pushliteral(L, "how");                       /* 1st argument */
    //  lua_getglobal(L, "t");                    /* table to be indexed */
    //  lua_getfield(L, -1, "x");        /* push result of t.x (2nd arg) */
    //  lua_remove(L, -2);                  /* remove 't' from the stack */
    //  lua_pushinteger(L, 14);                          /* 3rd argument */
    //  lua_call(L, 3, 1);     /* call 'f' with 3 arguments and 1 result */
    //  lua_setglobal(L, "a");                         /* set global 'a' */
    void OnGameInit()
    {
        lua_getglobal(L, "OnGameInit");                  
        lua_call(L, 0, 0);
    }

    void OnGameUpdate(double dt)
    {
        lua_getglobal(L, "OnGameUpdate");                  
        lua_pushnumber(L, dt);                  
        lua_call(L, 1, 0);
    }

    void OnGameDraw()
    {
        lua_getglobal(L, "OnGameDraw");                  
        lua_call(L, 0, 0);
    }
    
private:
    static lua_State* L;
};
