#pragma once
#include "defines.h"
#include "Game.h"
#include "Singleton.h"


/*用于控制Game 负责Game的Init Update Draw*/
class Window : public Singleton<Window>
{
public:
	friend Singleton<Window>;
	Window(int w,int h);
	~Window();
	
	void Show();
	GLFWwindow* GetGLFWwindow();
	static int GetWidth();
	 static int GetHeight();


private:
	static int m_Width;
	static int m_Height;
	GLFWwindow *p_Window;
	Game* p_Game;
	
	
};