#pragma once

#include "../defines.h"
#include "../defineTypes.h"
#include "Player.h"

class CombatSystem
{
public:
	CombatSystem();
	~CombatSystem();
	void Init(Player* self,Player* enemy);
	void Update();
	void Draw();
private:
	Player* m_Self;
	Player* m_Enemy;
};
