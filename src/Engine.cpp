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


Engine::~Engine()
{

}


Engine::Engine()
: Singleton<Engine>()
{

}


void Engine::Init()
{
	LuaVM::GetInstance()->Init();
	luaL_dostring(LuaVM::get(),R"
		print("hello world");
	");
	//mSence = new TestNetwork();
	mSence = new AlphaSence();
	// mSence = new Demo();
    InputManager::GetInstance()->SetKeyCallback();
    InputManager::GetInstance()->SetScrollCallback();
    InputManager::GetInstance()->SetMouseCallback();
    InputManager::GetInstance()->SetMouseButtonCallback();
	TimerManager::GetInstance();
}

void Engine::Update(double dt)
{
    m_DeltaTime = dt;
	TimerManager::GetInstance()->Update();
    mSence->Update();
}

void Engine::Draw()
{ 
    mSence->Draw();
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
void Engine::SetClient(Client* client)
{

	static_cast<Demo*>(mSence)->SetClient(client);
}
