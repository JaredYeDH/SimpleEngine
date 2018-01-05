#include "Engine.h"	
#include "Texture.h"
#include "SpriteBatch.h"
#include "CubeBatch.h"
#include "InputManager.h"
#include "Projects/TestSence.h"
#include "Projects/AlphaSence.h"
#include "Projects/Demo.h"
#include "Network/Message.h"

Engine::~Engine()
{

}


Engine::Engine()
: Singleton<Engine>()
{

}


void Engine::Init()
{
	//mSence = new TestNetwork();
	mSence = new AlphaSence();
	//mSence = new Demo();
    InputManager::GetInstance()->SetKeyCallback();
    InputManager::GetInstance()->SetScrollCallback();
    InputManager::GetInstance()->SetMouseCallback();
    InputManager::GetInstance()->SetMouseButtonCallback();

}

void Engine::Update(double dt)
{
    m_DeltaTime = dt;
    mSence->Update();

}

void Engine::Draw()
{ 
    mSence->Draw();

  
}
void Engine::DispatchMove(MoveMessage msg)
{
	static_cast<Demo*>(mSence)->OnMove(msg);
}
void Engine::SetClient(Client* client)
{

	static_cast<Demo*>(mSence)->SetClient(client);
}
