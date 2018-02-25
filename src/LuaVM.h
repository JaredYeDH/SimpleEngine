#pragma once
#include "defines.h"
#include "defineTypes.h"
#include "Singleton.h"

class LuaVM : public Singleton<LuaVM>
{
public:
    friend Singleton<LuaVM>;
    static lua_State* get(){return m_State;};
    void Init()
    {
        m_State = luaL_newstate();
    }
    
private:
    static lua_State* m_State;
}