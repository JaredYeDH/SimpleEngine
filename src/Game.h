#pragma once
#include <string>
#include "Engine.h"

/*
负责初始化Engine Update Draw
*/
class Game
{
public:
	Game(int w,int h,std::string title);
	~Game();
	void Start();
	void Update(double dt);
	void Draw();
	void End();

private:
	int mResWidth,mResHeight;
	std::string mTitle;
};