#include "defines.h"

//包含Window 创建出来一个window
#include "Window.h"
#include "global.h"
#include "Network/Client.h"
#include "Projects/Demo.h"
#include "Logger.h"
#include "LuaVM.h"
/*
Program负责window的外部交互 例如创建窗口 移动窗口 主要负责将操作系统上的外部操作转换成游戏里面的内部操作
*/
int main(int argc,char** argv)
{

    WINDOW_INSTANCE->Init(SCREEN_WIDTH, SCREEN_HEIGHT);
	WINDOW_INSTANCE->Show();
	return 0;
}
