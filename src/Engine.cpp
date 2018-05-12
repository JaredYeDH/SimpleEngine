#include "Engine.h"	
#include "core/Texture.h"
#include "SpriteBatch.h"
#include "CubeBatch.h"
#include "InputManager.h"
#include "Projects/TestSence.h"
#include "Projects/AlphaSence.h"
#include "Projects/Demo.h"
#include "Network/Message.h"
#include "LuaVM.h"

#include "imgui_impl_glfw_gl3.h"

Engine::~Engine()
{

}


Engine::Engine()
:m_LastTime(0),m_DeltaTime(0)
{
	
}

void Engine::Init()
{
	INPUT_MANAGER_INSTANCE->Init();
	LuaVM::GetInstance()->Init();
	// LuaVM::GetInstance()->doLuaString(R"(
    //               print("hello world");
    //               )" );
	
	//mSence = new TestNetwork();
	// mSence = new AlphaSence();
	mSence = new Demo();
    InputManager::GetInstance()->SetKeyCallback();
    InputManager::GetInstance()->SetScrollCallback();
    InputManager::GetInstance()->SetMouseCallback();
    InputManager::GetInstance()->SetMouseButtonCallback();
	TimerManager::GetInstance();


	LuaVM::GetInstance()->OnGameInit();
}

void Engine::Update()
{
	double now = glfwGetTime();
    m_DeltaTime = now - m_LastTime;
	m_LastTime = now;
	
	TimerManager::GetInstance()->Update();
    mSence->Update();
	LuaVM::GetInstance()->OnGameUpdate(m_DeltaTime);

	if (InputManager::GetInstance()->IsKeyUp(GLFW_KEY_F5) )
	{
		LuaVM::GetInstance()->CallSimpleFunc("GotoDebug");
	}
}

void Engine::Draw()
{ 	
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
    mSence->Draw();

	LuaVM::GetInstance()->OnGameDraw();
	
	ImGui::Render();
}

void Engine::Destroy()
{
	TimerManager::GetInstance()->DeleteSingleton();
	InputManager::GetInstance()->DeleteSingleton();
}
void Engine::DispatchMove(MoveMessage msg)
{
	static_cast<Demo*>(mSence)->OnMove(msg);
}

