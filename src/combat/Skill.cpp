#include "Skill.h"
#include "../Engine.h"
#include "../global.h"
#include "../Message.h"
#include "../simple_engine.h"

SkillManager::SkillManager()
{
	m_RandomID=-1;
	m_RandomIndex = 0;
	utils::tsv skillTSV(Environment::GetTSVPath("magic"));
	m_Skills.clear();
	for(int i=0;i<skillTSV.rows.size()-1;i++)
	{	
		auto row = skillTSV.cols[i];
		int id = std::stoi(row[0]);
		auto _was_id = row[2];
		uint32 was_id= std::stoul(_was_id, 0, 16);

        std::shared_ptr<Sprite2> sp = ResourceManager::GetInstance()->LoadWASSprite(ResourceManager::MAGICWDF, was_id);
		if(!sp->Error)
		{
			m_Skills[id] = FrameAnimation(sp);
		}
	}
}
