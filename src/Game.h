#pragma once
#include <string>
#include "Engine.h"
#include "Singleton.h"
/*
负责初始化Engine Update Draw
*/
class Game : public Singleton<Game>
{
public:
	Game();
	~Game();
	
	void Start();
	void Update();
	void Draw();
	void End();
};

#define GAME_INSTANCE Game::GetInstance()