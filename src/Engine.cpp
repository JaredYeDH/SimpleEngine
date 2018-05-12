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
#include "SceneManager.h"


Engine::Engine()
:m_LastTime(0),m_DeltaTime(0)
{
	
}

Engine::~Engine()
{

}



void Engine::Init()
{
	INPUT_MANAGER_INSTANCE->Init();
		
	SCENE_MANAGER_INSTANCE->Init();
	
   	TIMER_MANAGER_INTANCE; //init
	
	LUAVM_INSTANCE->Init();
}

void Engine::Update()
{
	double now = glfwGetTime();
    m_DeltaTime = now - m_LastTime;
	m_LastTime = now;
	
	TIMER_MANAGER_INTANCE->Update();

    SCENE_MANAGER_INSTANCE->Update();

	LUAVM_INSTANCE->OnGameUpdate(m_DeltaTime);
}

void Engine::Draw()
{ 	
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	SCENE_MANAGER_INSTANCE->Draw();
    
	LUAVM_INSTANCE->OnGameDraw();
	
	ImGui::Render();
}

void Engine::Destroy()
{
	INPUT_MANAGER_INSTANCE->DeleteSingleton();
		
	SCENE_MANAGER_INSTANCE->DeleteSingleton();
	
   	TIMER_MANAGER_INTANCE->DeleteSingleton(); 
	
	LUAVM_INSTANCE->DeleteSingleton();

}

