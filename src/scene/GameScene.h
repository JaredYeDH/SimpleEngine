#pragma once
#include "defines.h"
#include "defineTypes.h"
#include "simple_engine.h"

#include "../IUpdateDraw.h"
#include "../Event/Event.h"
#include "core/Texture.h"
#include "GameMap.h"
#include "Player.h"



class GameScene : public IUpdateDraw, public IMouseEvent
{
public:
	GameScene();
	~GameScene();
	int GetScreenWidth() { return SCREEN_WIDTH;}
	int GetScreenHeight() { return SCREEN_HEIGHT;}
	void Update() override;
	void Draw() override;
	void OnEvent(int button, int action, int mods) override;
private:
	void ProcessInput();
	
	GameMap* m_GameMapPtr;
	
	std::vector< Player * > m_NPCs;

	bool m_IsChangeState = false;
	double m_ChangeStateTimeInterval = 0;
	bool m_IsTestNpc0;
	
};


