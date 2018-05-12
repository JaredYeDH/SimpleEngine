#include "Window.h"

#include "global.h"
#include "core/Shader.h"
#include "Environment.h"
#include "InputManager.h"
#include "imgui_impl_glfw_gl3.h"

Window::Window()
:m_Width(0),m_Height(0)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

Window::~Window()
{
	
}

void Window::Init(int w,int h)
{
	m_Width = w ;
	m_Height = h;
	
	m_pWindow = glfwCreateWindow(m_Width, m_Height, WINDOW_TITLE, nullptr, nullptr);
	if(!m_pWindow) return;

	glfwMakeContextCurrent(m_pWindow);
	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glfwGetFramebufferSize(m_pWindow, &m_Width, &m_Height);
	glViewport(0, 0, m_Width, m_Height);

	glewExperimental = GL_TRUE;
	glewInit();

	ImGui_ImplGlfwGL3_Init(m_pWindow,true);
}

void Window::Show()
{	
	GAME_INSTANCE->Start();

    while (!glfwWindowShouldClose(m_pWindow))
    {
		glfwPollEvents();

		ImGui_ImplGlfwGL3_NewFrame();
		GAME_INSTANCE->Update();

		GAME_INSTANCE->Draw();
        glfwSwapBuffers(m_pWindow);
    }
	
	GAME_INSTANCE->End();
    glfwTerminate();
}

