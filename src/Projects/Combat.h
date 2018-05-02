#pragma once

#include "../defines.h"
#include "../defineTypes.h"
#include "Player.h"
#include "../tsv.h"
#include "../Singleton.h"
#include "FrameAnimation.h"
#include "../Random.h"

class SkillManager : public Singleton<SkillManager>
{
	friend class Singleton<SkillManager>;
public:
	FrameAnimation* GetSkillByID(int id){ 
		assert(m_Skills.find(id) != m_Skills.end());
		return &m_Skills[id]; 
	};
	FrameAnimation* GetRandomSkill(){
        std::cout << "m_RandomId ::" << m_RandomID << std::endl;
		return &m_Skills[m_RandomID];
	};

	void SetRandomSkillID(){ 
		m_RandomID=RANDOM_INSTANCE->NextInt(0,m_Skills.size()-1);
		// m_RandomID=21;
	};

protected:
	SkillManager();
private:
	int m_RandomID;
	std::map<int,FrameAnimation> m_Skills;
};
#define SKILL_MANAGER_INSTANCE SkillManager::GetInstance()


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
