#pragma once

#include "../defines.h"
#include "../defineTypes.h"
#include "Player.h"

class CombatSystem
{
public:
	CombatSystem();
	~CombatSystem();
	
	void AddEnemy(int pos,Player* enemy);
	void AddSelf(int pos,Player* self);
	void ProcessInput();
	void Update();
	void Draw();
private:
	std::vector<Player*> m_Ourselves;
	std::vector<Player*> m_Enemies;
};
