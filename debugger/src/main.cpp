#include "defines.h"
#include "Debugger.h"
/*
Debugger 
利用ASIO把 stdinput stdout 转化为 socket in socket out
这个socket是Debugger和VSCodeDebuggerAdapter之间的Socket

*/
int main(int argn,char** argv)
{
    int  port = 0;
    if (argn > 1){
        port = atoi(argv[1]);
    }
	DebugSession s(port);
	
	// DebugSession s(port);
// 		std::thread t([&io_context](){ io_context.run(); });

    
	// lua_State *L;
	// L = luaL_newstate();
	// luaL_openlibs(L);
	// std::cout << "server start" << std::endl;
	// std::string file(WORK_DIR"/../scripts/server/main.lua");
	// std::cout << file << std::endl;
	// luaL_dofile(L, file.c_str());
	// std::cout << "server end" << std::endl;
	// while (true); 
	return 0;
}

// char command[256];   
// int main()
// {   
// int rtn; /*子进程的返回数值*/   
// while(1) {   
// /* 从终端读取要执行的命令 */   
// printf( ">" );   
// fgets( command, 256, stdin );   
// command[strlen(command)-1] = 0;   
// if ( fork() == 0 ) {   
// /* 子进程执行此命令 */   
// execlp( command, command );   
// /* 如果exec函数返回，表明没有正常执行命令，打印错误信息*/   
// perror( command );
//     exit(0);
// }   
// else {   
// /* 父进程， 等待子进程结束，并打印子进程的返回值 */   
// wait ( &rtn );   
// printf( " child process return %d/n",rtn );
// }   
// }
//     return 0;
// }  
