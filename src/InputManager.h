#pragma once

#include "Singleton.h"
#include "Event/Event.h"

/*
管理输入 包括：鼠标移动，滚动 键盘按键等
*/
struct MousePos
{
	double x,y;
	MousePos(double x,double y) { this->x = x ; this->y = y ;}
};

class InputManager final : public Singleton<InputManager>
{
public:
	static void KeyCallbackFunc(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void ScrollCallbackFunc(GLFWwindow* window, double xoffset, double yoffset);
	static void MouseCallbackFunc(GLFWwindow* window, double xpos, double ypos);
 	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	friend Singleton<InputManager>;

	void Init(GLFWwindow* window);
	
    void SetKeyCallback();
    void SetScrollCallback();
    void SetMouseCallback();
    void SetMouseButtonCallback();

    bool IsKeyDown(int keyCode) { return s_Keys[keyCode]; }
    bool IsKeyUp(int keyCode) { return s_Keys[keyCode]; }

    double GetMouseX() { return s_MousePos.x; }
    double GetMouseY() { return s_MousePos.y; }

    void SetMouseEvent(IMouseEvent* event){ s_IMouseEvent = event; }


private:
	InputManager();
	~InputManager();

	GLFWwindow* m_pWindow;

	static bool	s_Keys[1024];

	static bool	s_FirstMouse;

	static MousePos s_MousePos;

	static IMouseEvent * s_IMouseEvent;

};

