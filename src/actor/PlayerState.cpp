#include "PlayerState.h"
#include "Player.h"
#include "../combat/Skill.h"

void BasePlayerCombatState::Enter(Player* )
{

}
void BasePlayerCombatState::Execute(Player* player)
{

}
void BasePlayerCombatState::Exit(Player* )
{

}
bool BasePlayerCombatState::OnMessage(Player*, const Telegram&)
{
	return false;
}

void PlayerCombatIdleState::Enter(Player* player) 
{
	player->SetActionID(4);
}

void PlayerCombatIdleState::Execute(Player* player) 
{
	
};

void PlayerCombatMoveState::Enter(Player* player) 
{
	m_bSent = false;
	player->SetActionID(11);
	player->SetPos(player->GetCombatPos());
	auto& playerFrame = player->GetCurrentPlayerFrame();
	auto& weaponFrame = player->GetCurrentWeaponFrame();
    Pos combatPos =player->GetCombatPos();
    Pos targetPos =player->GetCombatTargetPos();
    
	double dist_sqr = player->GetCombatDistSquare();
	double d = std::sqrt(dist_sqr) *1.0 / playerFrame.GetGroupFrameCount();
	player->SetVelocity(1100);
	double localVelocity = player->GetVelocity();
	playerFrame.SetFrameTimeBase(d/localVelocity);
	weaponFrame.SetFrameTimeBase(d/localVelocity);
	weaponFrame.ResetFrameTimeByGroupCount(playerFrame.GetGroupFrameCount());
}

void PlayerCombatMoveState::Execute(Player* player) 
{	
	double dt = ENGINE_INSTANCE->GetDeltaTime(); 
	double localVelocity = player->GetVelocity()*dt;
    Pos combatPos =player->GetCombatPos();
    Pos targetPos =player->GetCombatTargetPos();

    double dist_sqr = player->GetCombatDistSquare();
	//double time_base = 1.0/60*4;
	if (dist_sqr > localVelocity*localVelocity) {
		int degree = player->GetCombatAngle() ;
		//player->m_Dir = GMath::Astar_GetDir(degree);
		double stepRangeX = cos(DegreeToRadian(degree));
		double stepRangeY = sin(DegreeToRadian(degree));
        player->SetCombatPos(combatPos.x + stepRangeX*localVelocity , combatPos.y + stepRangeY*localVelocity);

		// SetDir(player->m_Dir);
	}
	else
	{
		player->SetVelocity(400);
		player->GetFSM()->ChangeState(PlayerCombatAttackState::GetInstance());
	}
}

bool PlayerCombatMoveState::OnMessage(Player* player, const Telegram& msg) 
{
	player->SetVelocity(400);
	player->GetFSM()->ChangeState(PlayerCombatAttackState::GetInstance());
	return true;
};

void PlayerCombatAttackState::Enter(Player* player) 
{
	player->SetActionID(6);
	auto& playerFrame = player->GetCurrentPlayerFrame();
	auto& weaponFrame = player->GetCurrentWeaponFrame();
	playerFrame.SetFrameTimeBase(1.0/60*5);
	weaponFrame.SetFrameTimeBase(1.0/60*5);
	weaponFrame.ResetFrameTimeByGroupCount(playerFrame.GetGroupFrameCount());
}
void PlayerCombatAttackState::Execute(Player* player) 
{
	if(player->GetPlayerFrames().find(6)!= player->GetPlayerFrames().end() )
	{
		
		auto& playerFrame = player->GetCurrentPlayerFrame();
		auto& weaponFrame = player->GetCurrentWeaponFrame();
		if(playerFrame.IsCurrentFrameChangedInUpdate() && playerFrame.GetLastFrame() % playerFrame.GetGroupFrameCount()== 3)
		{
			MESSAGE_DISPATCHER_INSTANCE->DispatchMessageX(2,player->GetID(),player->GetTargetID(),(int)MSG_PlayerCombatBeAttackedState,nullptr);
		}
		
		if(playerFrame.IsNextFrameRestart())
		{
			// player->SetCombatPos(player->GetPos());			
			player->SetCombatTargetPos(player->GetPos());

			player->GetFSM()->ChangeState(PlayerCombatBackState::GetInstance());
		}
	}
	else
	{
		player->GetFSM()->ChangeState(PlayerCombatIdleState::GetInstance());
	}
}
void PlayerCombatBackState::Enter(Player* player) 
{
	player->SetActionID(13);
	auto& playerFrame = player->GetCurrentPlayerFrame();
	auto& weaponFrame = player->GetCurrentWeaponFrame();
	double dist_sqr = player->GetCombatDistSquare() ;
	double d = std::sqrt(dist_sqr) *1.0 / playerFrame.GetGroupFrameCount();
	player->SetVelocity(950);
	double localVelocity = player->GetVelocity();
	playerFrame.SetFrameTimeBase(d/localVelocity);
	weaponFrame.SetFrameTimeBase(d/localVelocity);
	player->ReverseDir();
	weaponFrame.ResetFrameTimeByGroupCount(playerFrame.GetGroupFrameCount());
}
void PlayerCombatBackState::Execute(Player* player) 
{
	double dt = ENGINE_INSTANCE->GetDeltaTime(); 
	double localVelocity = player->GetVelocity()*dt;
	if (player->GetCombatDistSquare() > localVelocity*localVelocity) {
		int degree = player->GetCombatAngle();
		//player->m_Dir = GMath::Astar_GetDir(degree);
		double stepRangeX = cos(DegreeToRadian(degree));
		double stepRangeY = sin(DegreeToRadian(degree));
		Pos combatPos = player->GetCombatPos();
		player->SetCombatPos( combatPos.x +  stepRangeX*localVelocity,combatPos.y +  stepRangeY*localVelocity);
		
		// SetDir(player->m_Dir);
	}
	else
	{
		player->ReverseDir();
		player->GetFSM()->ChangeState(PlayerCombatIdleState::GetInstance());
	}
}


void PlayerCombatBeAttackedState::Enter(Player* player) 
{
	player->SetActionID(10);
    auto& playerFrame = player->GetCurrentPlayerFrame();
    auto& weaponFrame = player->GetCurrentWeaponFrame();
    playerFrame.SetFrameTimeBase(1.0 / 60 * 4 );
    weaponFrame.SetFrameTimeBase(1.0 / 60 * 4 );
	
}
void PlayerCombatBeAttackedState::Execute(Player* player) 
{
	if(player->GetPlayerFrames().find(11)!= player->GetPlayerFrames().end() )
	{
		auto& playerFrame = player->GetCurrentPlayerFrame();
		auto& weaponFrame = player->GetCurrentWeaponFrame();
		
		if(playerFrame.IsNextFrameRestart())
		{
			player->GetFSM()->ChangeState(PlayerCombatIdleState::GetInstance());
		}
	}
}


void PlayerCombatBeCastAttackedState::Enter(Player* player) 
{
	auto& playerFrame = player->GetCurrentPlayerFrame();
	FrameAnimation* frame = SKILL_MANAGER_INSTANCE->GetRandomSkill();
	frame->SetFrameTimeBase(1.0 / 60 * 4 );
	//int x = std::floor(player->GetCombatPos().x - frame->GetKeyX());
	//int y = std::floor(player->GetCombatPos().y - frame->GetKeyY());
	//frame->SetPos({x,y});
	player->SetSkillFrame(frame);
}

void PlayerCombatBeCastAttackedState::Execute(Player* player) 
{
	if(player->GetPlayerFrames().find(11)!= player->GetPlayerFrames().end() )
	{
        auto& skillFrame = player->GetSkillFrame();
        if(skillFrame.GetCurrentFrame() == std::floor(skillFrame.GetGroupFrameCount()/3.0*2))
		{
			player->GetFSM()->ChangeState(PlayerCombatBeAttackedState::GetInstance());
		}
	}
}


bool PlayerCombatGlobalState::OnMessage(Player* player, const Telegram& msg)
{
	if(msg.MsgType == MSG_PlayerCombatBeAttackedState)
	{
		player->GetFSM()->ChangeState(PlayerCombatBeAttackedState::GetInstance());
		return true;
	}
	else if(msg.MsgType == MSG_PlayerCombatBeCastAttackedState)
	{
		player->GetFSM()->ChangeState(PlayerCombatBeCastAttackedState::GetInstance());
		return true;
	}
	return false;
}
void PlayerCombatGlobalState::Enter(Player* player) 
{
}
void PlayerCombatGlobalState::Execute(Player* player) 
{
}
void PlayerCombatCastAttackState::Enter(Player* player) 
{
	player->SetActionID(7);
	auto& playerFrame = player->GetCurrentPlayerFrame();
	auto& weaponFrame = player->GetCurrentWeaponFrame();
	playerFrame.SetFrameTimeBase(1.0/60*5);
	weaponFrame.SetFrameTimeBase(1.0/60*5);
	weaponFrame.ResetFrameTimeByGroupCount(playerFrame.GetGroupFrameCount());
}
void PlayerCombatCastAttackState::Execute(Player* player) 
{
	if(player->GetPlayerFrames().find(7)!= player->GetPlayerFrames().end() )
	{
		
		auto& playerFrame = player->GetCurrentPlayerFrame();
		auto& weaponFrame = player->GetCurrentWeaponFrame();
		if(playerFrame.IsCurrentFrameChangedInUpdate() && playerFrame.GetLastFrame() % playerFrame.GetGroupFrameCount()== playerFrame.GetGroupFrameCount()/3)
		{
			SKILL_MANAGER_INSTANCE->SetRandomSkillID();
			
			std::set<int> enemySet;
			for(int i=0;i<10;i++)
            {
                int a = RANDOM_INSTANCE->NextInt(10,19);
				enemySet.insert(a);
			}
			int i = 0;
			for (auto it = enemySet.begin(); it != enemySet.end(); ++it)
			{
				int enemy = *it;
				MESSAGE_DISPATCHER_INSTANCE->DispatchMessageX(i,player->GetID(),enemy,MSG_PlayerCombatBeCastAttackedState,nullptr);	
				i=i+2;
			}
			
		}
		
		if(playerFrame.IsNextFrameRestart())
		{
			player->GetFSM()->ChangeState(PlayerCombatIdleState::GetInstance());
		}
	}
	else
	{
		player->GetFSM()->ChangeState(PlayerCombatIdleState::GetInstance());
	}
}
