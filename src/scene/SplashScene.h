#pragma once
#include "defines.h"
#include "defineTypes.h"
#include "simple_engine.h"

#include "../IUpdateDraw.h"
#include "../Event/Event.h"
#include "core/Texture.h"
#include "GameMap.h"
#include "../actor/Player.h"
#include "BaseScene.h"

class SplashScene  : public BaseScene
{
public:
	enum State
	{
		Splash,
		Game
	};

	SplashScene(int id,String name);
	~SplashScene();
	void Init() override;
	void Update() override;
	void Draw() override;
private:
	int m_State;
};


