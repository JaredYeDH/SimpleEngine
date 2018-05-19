#pragma once

#include "Singleton.h"
#include "Event/Event.h"
#include <functional>

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

	bool IsKeyDown(int keyCode) { return s_Keys[keyCode]; }
    bool IsKeyUp(int keyCode) { return s_Keys[keyCode]; }
	

	int GetKeyState(int keyCode) { return s_KeyStates[keyCode]; }

    double GetMouseX() { return s_MousePos.x; }
    double GetMouseY() { return s_MousePos.y; }


	void RegisterOnKeyClickEvent(int keyCode,std::function<void()> callback) {s_ClickEvents[keyCode] = callback;}
	void RegisterOnKeyDownEvent(int keyCode, std::function<void()> callback) { s_KeyDownEvents[keyCode] = callback; }
	void RegisterOnKeyUpEvent(int keyCode, std::function<void()> callback) { s_KeyUpEvents[keyCode] = callback; }
private:
	InputManager();
	~InputManager();

	GLFWwindow* m_pWindow;

	static bool	s_Keys[1024];

	static bool s_bKeyBlock;

	static int	s_KeyStates[1024];

	static bool	s_FirstMouse;

	static MousePos s_MousePos;

	static std::map<int,std::function<void()>> s_ClickEvents;
	static std::map<int, std::function<void()>> s_KeyDownEvents;
	static std::map<int, std::function<void()>> s_KeyUpEvents;
};

#define INPUT_MANAGER_INSTANCE InputManager::GetInstance()
