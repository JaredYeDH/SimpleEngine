#pragma once

#include "Singleton.h"
#include "Event/Event.h"
#include <functional>

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
	friend Singleton<InputManager>;  // cause private ctor dtor

	static void KeyCallbackFunc(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void ScrollCallbackFunc(GLFWwindow* window, double xoffset, double yoffset);
	static void MouseCallbackFunc(GLFWwindow* window, double xpos, double ypos);
 	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	void Init();
	
    void SetKeyCallback();
    void SetScrollCallback();
    void SetMouseCallback();
    void SetMouseButtonCallback();

    // bool IsKeyDown(int keyCode) { return s_KeyStates[keyCode] == GLFW_PRESS; }
    // bool IsKeyUp(int keyCode) { return s_KeyStates[keyCode] == GLFW_RELEASE; }

	bool IsKeyDown(int keyCode) { return s_Keys[keyCode]; }
    bool IsKeyUp(int keyCode) { return s_Keys[keyCode]; }
	

	int GetKeyState(int keyCode) { return s_KeyStates[keyCode]; }
	//void BindKey(int keyCode, const std::function<void(int key,Object2D& )> & func  );

    double GetMouseX() { return s_MousePos.x; }
    double GetMouseY() { return s_MousePos.y; }

    void SetMouseEvent(IMouseEvent* event){ s_IMouseEvent = event; }

	void RegisterOnKeyClickEvent(int keyCode,std::function<void()> callback) {s_ClickEvents[keyCode] = callback;}
private:
	InputManager();
	~InputManager();

	GLFWwindow* m_pWindow;

	static bool	s_Keys[1024];

	static bool s_bKeyBlock;

	static int	s_KeyStates[1024];

	static bool	s_FirstMouse;

	static MousePos s_MousePos;

	static IMouseEvent * s_IMouseEvent;
	static std::map<int,std::function<void()>> s_ClickEvents;
};

#define INPUT_MANAGER_INSTANCE InputManager::GetInstance()