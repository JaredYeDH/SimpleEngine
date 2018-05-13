#pragma once

#include "../defines.h"
#include "../defineTypes.h"
#include "../actor/Player.h"
#include "../Singleton.h"


class CombatSystem : public Singleton<CombatSystem>
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

#define COMBAT_SYSTEM_INSTANCE CombatSystem::GetInstance()
