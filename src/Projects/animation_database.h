#pragma once
#include "../defines.h"
#include "../defineTypes.h"
#include "../tsv.h"

namespace utils
{
	class AnimDB
	{
	public:
		int query(int role_id,int action_id  ,int action_weapon_id,int weapon_id)
		{
            for(int i=0;i<role_wasTSV.rows.size()-1;i++)
            {
                auto row = role_wasTSV.cols[i];
                int _id = atoi( row[0].c_str());
                int _role_id =  atoi( row[1].c_str());
                int _action_id =  atoi( row[2].c_str());
                int _action_weapon_id =  atoi( row[3].c_str());
                int _weapon_id =  atoi( row[4].c_str());
                auto _was_id =   row[5];
                if (role_id == _role_id && weapon_id == _weapon_id && action_id == _action_id
                    &&action_weapon_id  == _action_weapon_id)
				{
					int num = std::stoi(_was_id, 0, 16);
                    return num;	
				}
				
            }
            return -1;
        };
		
		AnimDB()
		:playerTSV(Environment::GetTsvPath("player")),
		actionTSV(Environment::GetTsvPath("action")),
		roleTSV(Environment::GetTsvPath("role")),
		role_wasTSV(Environment::GetTsvPath("role_was")),
		weaponTSV(Environment::GetTsvPath("weapon"))
		{
			
		};

		~AnimDB(){};
	private:
		using AnimTable = std::map<int , int >; // anim_id -> was_id
		AnimTable m_AnimTable;
		utils::tsv playerTSV;
		utils::tsv actionTSV;
		utils::tsv roleTSV;
		utils::tsv role_wasTSV;
		utils::tsv weaponTSV;
	};
};
