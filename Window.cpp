#include "Window.h"

#include "global.h"
#include "Shader.h"
#include "Environment.h"
#include "InputManager.h"
#include "imgui_impl_glfw_gl3.h"

Window::Window(int width,int height)
{
    // Init GLFW
	glfwInit();
    // Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
	p_Window = glfwCreateWindow(width, height, WINDOW_TITLE, nullptr, nullptr);
	glfwMakeContextCurrent(p_Window);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	ImGui_ImplGlfwGL3_Init(p_Window,true);

    // Define the viewport dimensions
	// int width, height;
	glfwGetFramebufferSize(p_Window, &width, &height);
	glViewport(0, 0, width, height);

    glfwSetInputMode(p_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    p_Game = new Game(width,height,WINDOW_TITLE);
    InputManager::GetInstance()->Init(p_Window);
	p_Game->Start();
}

Window::~Window()
{
	
}

GLfloat deltaTime = 0;
GLfloat lastFrame = 0.0f;
void Window::Show()
{	
	 // Game loop
    while (!glfwWindowShouldClose(p_Window))
    {
        GLfloat currentFrame = glfwGetTime();
       
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
      
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
    	glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();

        p_Game->Update(deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        p_Game->Draw();
		
        // Swap the screen buffers
        glfwSwapBuffers(p_Window);
    }
    
    p_Game->End();
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
}


GLFWwindow* Window::GetGLFWwindow()
{
    return p_Window;
}

