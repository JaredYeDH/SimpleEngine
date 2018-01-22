// // static int Lua_DrawCircle(lua_State *L)
// // {
// //     printf("static int Lua_DrawCircle(lua_State *L)");
// //     int x;
// //     int y;
// //     int radius;
// //     x = luaL_checkinteger(L, 1);
// //     y = luaL_checkinteger(L, 2);
// //     radius = luaL_checkinteger(L, 3);
// //     printf("%d %d %d\n",x,y,radius);
// //    	Circle* c=new Circle(100,100,88);
// // 	c->Color() = Vec4(1,0.5,0.2,1);
// // 	m_Render.AddObject(c);
// //     lua_pushnumber(L,0);
// //     return 0;
// // }

// lua_State *L ;
// L= luaL_newstate();
// 	luaopen_base(L);
// 	luaopen_table(L);
// 	luaopen_package(L);
// 	luaopen_io(L);
// 	luaopen_string(L);

// 	luaL_openlibs(L);


//     lua_pushcclosure(L,Lua_DrawCircle,0);
//     lua_setglobal(L, "DrawCircle");


//     /*
// 	using string = std::string;

//     string luafile(Environment::GetAbsPath("/scripts/Test.lua"));
// 	if(luaL_loadfile(L,luafile.c_str())
// 		|| lua_pcall(L,0,0,0) )
// 		{
// 			const char* error = lua_tostring(L,-1);
//             std::cout<<string(error)<<std::endl;
// 			return ;
// 		}
// 	int a= 0 ;
// 	int b =0;
// 	lua_getglobal(L,"a");
// 	if (!lua_isnumber(L,-1))
// 	{
//         std::cout << "not number " << std::endl;
// 		return;
// 	}

// 	a = lua_tonumber(L,-1);

// 	lua_getglobal(L,"b");
// 	b = lua_tonumber(L,-1);
//     std::cout << a << "   b : " << b  << std::endl;
// 	lua_close(L);


//      */

// string luafile(Environment::GetAbsPath("/scripts/Test3.lua"));
//     if(luaL_loadfile(L,luafile.c_str())
//        || lua_pcall(L,0,0,0) )
//     {
//         const char* error = lua_tostring(L,-1);
//         std::cout<<string(error)<<std::endl;
//         return ;
//     }