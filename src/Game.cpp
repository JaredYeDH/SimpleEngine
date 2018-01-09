#include "Game.h"	


Game::Game(int w,int h,std::string title)
{
	mResWidth =  w;
	mResHeight = h;
	mTitle = title;
}

Game::~Game()
{

}


void Game::Start()
{
	Engine::GetInstance()->Init();
 
}

void  Game::Update(double dt)
{
	Engine::GetInstance()->Update(dt);
}

void  Game::Draw()
{

	Engine::GetInstance()->Draw();

}

void  Game::End()
{

}