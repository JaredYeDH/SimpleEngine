#include "InputManager.h"

bool InputManager::s_Keys[1024] = {0};

bool InputManager::s_FirstMouse = true;

MousePos InputManager::s_MousePos(0,0);

IMouseEvent* InputManager::s_IMouseEvent(nullptr);

void InputManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(s_IMouseEvent != nullptr)
    {
        s_IMouseEvent->OnEvent(button,action,mods);
    }
}

void InputManager::KeyCallbackFunc(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if(action == GLFW_PRESS)
        s_Keys[key] = true;
    else if(action == GLFW_RELEASE)
        s_Keys[key] = false;	
}

void InputManager::ScrollCallbackFunc(GLFWwindow* window, double xoffset, double yoffset)
{
    
}

void InputManager::MouseCallbackFunc(GLFWwindow* window, double xpos, double ypos)
{
    if(s_FirstMouse)
    {
        s_MousePos.x = xpos;
        s_MousePos.y = ypos;
        s_FirstMouse = false;
    }

    GLfloat xoffset = xpos - s_MousePos.x;
    GLfloat yoffset = s_MousePos.y - ypos;  // Reversed since y-coordinates go from bottom to left
    
    s_MousePos.x = xpos;
    s_MousePos.y = ypos;
    
}

InputManager::InputManager()
:Singleton<InputManager>()
{
}

InputManager::~InputManager()
{
	m_pWindow = nullptr;
}

void InputManager::Init(GLFWwindow* window)
{
	m_pWindow = window;
}

void InputManager::SetKeyCallback()
{
    glfwSetKeyCallback(m_pWindow, KeyCallbackFunc);
}

void InputManager::SetScrollCallback()
{
    glfwSetScrollCallback(m_pWindow, ScrollCallbackFunc);   
}

void InputManager::SetMouseCallback()
{
	glfwSetCursorPosCallback(m_pWindow, MouseCallbackFunc);
}

void InputManager::SetMouseButtonCallback()
{
    glfwSetMouseButtonCallback(m_pWindow, mouse_button_callback);
}


 





