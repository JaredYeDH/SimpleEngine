#include "AlphaSence.h"
#include "core/Texture.h"


#include <iostream>



static int Lua_DrawCircle(lua_State *L)
{
    printf("static int Lua_DrawCircle(lua_State *L)");
    int x;
    int y;
    int radius;

    x = luaL_checkinteger(L, 1);
    y = luaL_checkinteger(L, 2);
    radius = luaL_checkinteger(L, 3);
    printf("%d %d %d\n",x,y,radius);
   // DrawCircle(x,y,radius,Vec4(0,1,0,1),false);

    lua_pushnumber(L,0);
    return 0;
}
lua_State *L ;
AlphaSence::AlphaSence()
{
	//TestTimer
	//TimerManager::GetInstance()->CreateTimer("Deda2", 1000, true, false, [this](){
	//	printf("void AlphaSence::Update(4000)\n");
	//	TimerManager::GetInstance()->CreateTimer("Deda3", 16, true, true,
	//		[](){DrawCircle(250, 150, 100, Vec4(0, 1, 0, 1), false);}	
	//	);
	//});
	L= luaL_newstate();
	luaopen_base(L);
	luaopen_table(L);
	luaopen_package(L);
	luaopen_io(L);
	luaopen_string(L);

	luaL_openlibs(L);


    lua_pushcclosure(L,Lua_DrawCircle,0);
    lua_setglobal(L, "DrawCircle");


    /*
	using string = std::string;

    string luafile(Environment::GetAbsPath("/scripts/Test.lua"));
	if(luaL_loadfile(L,luafile.c_str())
		|| lua_pcall(L,0,0,0) )
		{
			const char* error = lua_tostring(L,-1);
            std::cout<<string(error)<<std::endl;
			return ;
		}
	int a= 0 ;
	int b =0;
	lua_getglobal(L,"a");
	if (!lua_isnumber(L,-1))
	{
        std::cout << "not number " << std::endl;
		return;
	}

	a = lua_tonumber(L,-1);

	lua_getglobal(L,"b");
	b = lua_tonumber(L,-1);
    std::cout << a << "   b : " << b  << std::endl;
	lua_close(L);


     */

    using string = std::string;
    printf("");
    string luafile(Environment::GetAbsPath("/scripts/Test3.lua"));
    if(luaL_loadfile(L,luafile.c_str())
       || lua_pcall(L,0,0,0) )
    {
        const char* error = lua_tostring(L,-1);
        std::cout<<string(error)<<std::endl;
        return ;
    }

}

AlphaSence::~AlphaSence()
{
	
}

void AlphaSence::Update() 
{
    lua_settop(L,0);
    lua_getglobal(L,"OnUpdate");
    lua_pcall(L,0,0,0);

}



void AlphaSence::Draw() 
{
	//DrawLine(Vec2(50, 150), Vec2(200, 150), Vec4(1,0,0,1));

	//DrawRect(50,50,100,50, Vec4(0, 1, 0, 1));
//
	//DrawRect(250, 150, 100, 50, Vec4(0, 1, 0, 1));
	
	//DrawCircle(200, 200, 50, Vec4(0, 1, 0, 1));

	//DrawLine(Vec2(0, 100), Vec2(200, 50), Vec4(1, 0, 0, 1));

}

