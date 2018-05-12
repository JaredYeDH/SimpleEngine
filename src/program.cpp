#include "global.h"
#include "Window.h"

int main(int argc,char** argv)
{

    WINDOW_INSTANCE->Init(SCREEN_WIDTH, SCREEN_HEIGHT);
	WINDOW_INSTANCE->Show();
	return 0;
}
