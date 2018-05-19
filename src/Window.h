#pragma once
#include "defines.h"
#include "Game.h"
#include "Singleton.h"

class Window : public Singleton<Window>
{
public:
	Window();
	~Window();
	
	void Init(int w,int h);
	void Show();

	GLFWwindow* GetGLFWwindow(){ return m_pWindow;};
	int GetWidth() { return m_Width;};
	int GetHeight(){ return m_Height;};
private:
	double m_MouseX;
	double m_MouseY;
	int m_Width;
	int m_Height;
	GLFWwindow *m_pWindow;
};


#define WINDOW_INSTANCE Window::GetInstance()