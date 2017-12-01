#include "defines.h"

//包含Window 创建出来一个window
#include "Window.h"
#include "global.h"
#include "Network/Client.h"
#include "Projects/Demo.h"
#include "Logger.h"

/*
Program负责window的外部交互 例如创建窗口 移动窗口 主要负责将操作系统上的外部操作转换成游戏里面的内部操作
*/
int Demo::g_Id = 0;
int Demo::g_Id2 = 0;
int main(int argc,char** argv)
{
	/*根据配置创建出来一个窗口，窗口的作用是初始化游戏 以及结束游戏 这里相当于说创建一个游戏窗口*/

	asio::io_context io_context;
	auto work = asio::make_work_guard(io_context);
	std::thread t([&io_context]() { io_context.run(); });
	
	tcp::resolver resolver(io_context);
	auto endpoints = resolver.resolve("localhost", "999");
	
	Client* client = new Client(io_context, endpoints);

	if (argc < 3)
	{
		Demo::g_Id = 1;
		Demo::g_Id2 = 11;
	}
	else
	{
		Demo::g_Id = atoi(argv[1]);
		Demo::g_Id2 = atoi(argv[2]);
	}


	Window* p_Window = new  Window(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	Engine::GetInstance()->SetClient(client);
	
	
	Logger::Print("g id : %d", Demo::g_Id);
	p_Window->Show();

	io_context.stop();
	t.join();

	

	




	return 0;
}