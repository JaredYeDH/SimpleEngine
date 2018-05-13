#pragma once

#include "../defines.h"
#include "../defineTypes.h"
#include "../actor/Player.h"
#include "../animation/FrameAnimation.h"

class SkillManager : public Singleton<SkillManager>
{
	friend class Singleton<SkillManager>;
public:
	FrameAnimation* GetSkillByID(int id){ 
		assert(m_Skills.find(id) != m_Skills.end());
		return &m_Skills[id]; 
	};
	FrameAnimation* GetRandomSkill(){
		int i=0;
		for(auto& it : m_Skills)
		{
			if(i++ == m_RandomIndex) return &(it.second);
		}
		return nullptr;
	};

	void SetRandomSkillID(){ 

		m_RandomIndex=RANDOM_INSTANCE->NextInt(0,m_Skills.size()-1);
	};

protected:
	SkillManager();
private:
	int m_RandomID;
	int m_RandomIndex;
	std::map<int,int> m_SkillsIDMap;
	std::map<int,FrameAnimation> m_Skills;
};
#define SKILL_MANAGER_INSTANCE SkillManager::GetInstance()
