#pragma once
#include "../tsv.h"

namespace utils
{

	struct RowWasRow
	{
		int id;
		int role_id;
		int	action_id;
		int	action_weapon_id;
		int	weapon_id;
		int64 was_id;



    };
	class AnimDB
	{
	public:
		int64 query(int role_id,int action_id  ,int action_weapon_id,int weapon_id)
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
                    int64 num = std::stol(_was_id, 0, 16);
                    return num;	
				}
				
            }
            return -1;
        };
		
		RowWasRow* getRowWas(int64 was_id)
		{
			if (was_id == -1) return nullptr;
			for(auto& row : m_Data)
			{
				if(row.was_id == was_id)
					return &row;
			}
			return nullptr;
		}


		void printInfo(RowWasRow* data){
			if(data == nullptr) return ;
			std::cout << "PlayerName:" << roleTSV.val(data->role_id-1,1) << '\t';
			std::cout << "ActionName:" << actionTSV.val(data->action_id-1,1) << '\t';
			// std::cout << "ActionWeaponName:" << playerTSV[data->role_id].name<< '\t';
			std::cout << "WeaponName:" << weaponTSV.val(data->action_weapon_id-1,1)<< '\t' << std::endl;
			
		}


		AnimDB()
		:playerTSV(Environment::GetTsvPath("player")),
		actionTSV(Environment::GetTsvPath("action")),
		roleTSV(Environment::GetTsvPath("role")),
		role_wasTSV(Environment::GetTsvPath("role_was")),
		weaponTSV(Environment::GetTsvPath("weapon"))
		{

			m_Data.resize(role_wasTSV.rows.size()-1);
			m_Data.clear();
			
			for(int i=0;i<role_wasTSV.rows.size()-1;i++)
            {
                auto row = role_wasTSV.cols[i];

				RowWasRow rowData;
                rowData.id = atoi( row[0].c_str());
                rowData.role_id =  atoi( row[1].c_str());
                rowData.action_id =  atoi( row[2].c_str());
                rowData.action_weapon_id =  atoi( row[3].c_str());
                rowData.weapon_id =  atoi( row[4].c_str());
                auto _was_id =   row[5];
				rowData.was_id= std::stol(_was_id, 0, 16);
				
				m_Data.push_back(rowData);
            }
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
		std::vector<RowWasRow> m_Data;
	};
};
