#include "Game.h"	


Game::Game()
{

}

Game::~Game()
{

}


void Game::Start()
{
	ENGINE_INSTANCE->Init();
}

void  Game::Update()
{
	ENGINE_INSTANCE->Update();
}

void  Game::Draw()
{
	ENGINE_INSTANCE->Draw();
}

void  Game::End()
{
	ENGINE_INSTANCE->Destroy();
}