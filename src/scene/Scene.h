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

class Scene  : public BaseScene
{
public:
	Scene(int id,String name);
	~Scene();
	void Init() override;
	void Update() override;
	void Draw() override;
private:
	void ProcessInput();
	
	GameMap* m_GameMapPtr;

	bool m_IsChangeState = false;
	double m_ChangeStateTimeInterval = 0;
};


