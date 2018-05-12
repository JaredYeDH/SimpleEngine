#include "Player.h"
#include "ResourceManager.h"
#include "Demo.h"
#include "Logger.h"
#include "global.h"
#include "../Message.h"
#include "Combat.h"
#include "../Random.h"
//
//map 1501.map
//shape.wdf 49386FCE 54F3FC94
//shape.wd3 DF749306 1BEC0D8A

// PlayerId : 1-12 
// WeaponId : 0-160

//shape.wdf A16A06FF 4FBA48B8 
//shape.wd3 72013AF5 F2FB1AFA


Player::Player(int roleID):
m_RoleID(roleID),
m_WeaponID(1),
m_ActionID(1),
m_HasWeapon(false),
m_RoleTSV(Environment::GetTsvPath("role")),
m_WeaponTSV(Environment::GetTsvPath("weapon")),
m_ActionTSV(Environment::GetTsvPath("action")),
m_IsMove(false),
m_MoveVelocity(400),
m_UpdateDelta(0),
m_MoveList(),
m_BackupMoveList(),
m_MoveToCalled(false),
m_AnimDB(),
m_bInCombat(false),
m_CombatPos({0.0f,0.0f}),
m_CombatTargetPos({0.0f,0.0f})
{
	LogInfo();
	ChangeRole(m_RoleID);
	m_pFSM = new StateMachine<Player>(this);
	m_pFSM->SetCurrentState(PlayerCombatIdleState::GetInstance());
	m_pFSM->SetGlobalState(PlayerCombatGlobalState::GetInstance());
}


Player::Player()
:Player(0)
{
};

Player::~Player()
{

}
void Player::ReloadFrames()
{
	m_PlayerFrames.clear();
	for(int actionID=1;actionID<20;actionID++)
	{
		uint32 wasID = m_AnimDB.query(m_RoleID,actionID,m_RoleID == 3?7:3,-1);
		if(wasID != -1)
		{
			m_AnimDB.printInfo(wasID);
            auto sprite = ResourceManager::GetInstance()->LoadWdfSprite(wasID);
			m_PlayerFrames.insert(std::make_pair(actionID,FrameAnimation(sprite)));
		}
	}
	m_WeaponFrames.clear();
	for(int actionID=1;actionID<20;actionID++)
	{
		uint32 wasID = m_AnimDB.query(m_RoleID,actionID,m_WeaponID,90);
		if(wasID != -1)
		{
			m_AnimDB.printInfo(wasID);
            auto sprite = ResourceManager::GetInstance()->LoadWdfSprite(wasID);
			m_WeaponFrames.insert(std::make_pair(actionID,FrameAnimation(sprite)));
		}
	}
}
void Player::ChangeRole()
{
	ChangeRole(m_RoleID == 0 ? 3 :( (m_RoleID == 3) ? 4 : 3 ));
}
void Player::ChangeRole(int roleID)
{
	m_RoleID = roleID;
	m_PlayerFrames.clear();
	for(int actionID=1;actionID<20;actionID++)
	{
		uint32 wasID = m_AnimDB.query(m_RoleID,actionID,m_RoleID == 3?7:3,-1);
		if(wasID != -1)
		{
			m_AnimDB.printInfo(wasID);
            auto sprite = ResourceManager::GetInstance()->LoadWdfSprite(wasID);
			FrameAnimation frame(sprite);
			frame.ResetAnim(m_Dir);
			m_PlayerFrames.insert(std::make_pair(actionID,frame));
			if(m_WeaponFrames.find(actionID) != m_WeaponFrames.end())
			{
				m_WeaponFrames[actionID].ResetAnim(m_Dir);
				m_WeaponFrames[actionID].ResetFrameTimeByGroupCount(m_PlayerFrames[actionID].GetGroupFrameCount());
			}
		}
	}
	
}

void Player::ChangeWeapon()
{
	ChangeWeapon(0);
}

void Player::ChangeWeapon(int WeaponID )
{
	m_WeaponID = WeaponID == 0? ( m_RoleID == 3 ? 7 : 3) : WeaponID;
	m_WeaponFrames.clear();
	for(int actionID=1;actionID<20;actionID++)
	{
		uint32 wasID = m_AnimDB.query(m_RoleID,actionID,m_WeaponID,90);
		if(wasID != -1)
		{
			m_AnimDB.printInfo(wasID);
			auto sprite = ResourceManager::GetInstance()->LoadWdfSprite(wasID);
			FrameAnimation frame(sprite);
			frame.ResetAnim(m_Dir);
			m_WeaponFrames.insert(std::make_pair(actionID,frame));
			if(m_PlayerFrames.find(actionID) != m_PlayerFrames.end())
			{
				m_PlayerFrames[actionID].ResetAnim(m_Dir);
				m_WeaponFrames[actionID].ResetFrameTimeByGroupCount(m_PlayerFrames[actionID].GetGroupFrameCount());
			}
		}
	}
}

void Player::ChangeAction()
{
	m_ActionID++;
	if(m_ActionID > 20)m_ActionID = 1;
}

void Player::ChangeAction(int actionID)
{
	m_ActionID = actionID;
}

void Player::ReverseDir()
{
	switch(m_Dir)
	{
		case 6: m_Dir=4;break;
		case 4: m_Dir=6;break;
		case 5: m_Dir=7;break;
		case 7: m_Dir=5;break;
		case 3: m_Dir=1;break;
		case 1: m_Dir=3;break;
		case 2: m_Dir=0;break;
		case 0: m_Dir=2;break;
	}
	SetDir(m_Dir);
}

void Player::SaveFrame(int index)
{
	ResourceManager::GetInstance()->SaveWdfSprite(0x49386FCE);
}

void Player::OnUpdate(double dt)
{
	if(m_bInCombat)
	{
		
        // double localVelocity = m_MoveVelocity*dt;
		// if (GMath::Astar_GetDistanceSquare(m_CombatPos.x, m_CombatPos.y, m_CombatTargetPos.x, m_CombatTargetPos.y) > localVelocity*localVelocity) {
		// 	int degree = GMath::Astar_GetAngle(m_CombatPos.x, m_CombatPos.y, m_CombatTargetPos.x, m_CombatTargetPos.y);
		// 	//m_Dir = GMath::Astar_GetDir(degree);
		// 	double stepRangeX = cos(DegreeToRadian(degree));
		// 	double stepRangeY = sin(DegreeToRadian(degree));

		// 	m_CombatPos.x += stepRangeX*5;
		// 	m_CombatPos.y += stepRangeY*5;
		// 	//dt=dt/5;
		// 	// SetDir(m_Dir);
		// }
		// if(m_PlayerFrames.find(m_ActionID)!= m_PlayerFrames.end() )
		// {
		// 	m_PlayerFrames[m_ActionID].OnUpdate(dt);
		// 	if(m_WeaponFrames.find(m_ActionID)!= m_WeaponFrames.end() )
		// 	{
		// 		m_WeaponFrames[m_ActionID].OnUpdate(dt);
		// 	}
		// }
		m_pFSM->Update();
		if(m_PlayerFrames.find(m_ActionID)!= m_PlayerFrames.end() )
		{
			m_PlayerFrames[m_ActionID].OnUpdate(dt);
			if(m_WeaponFrames.find(m_ActionID)!= m_WeaponFrames.end() )
			{
				m_WeaponFrames[m_ActionID].OnUpdate(dt);
			}
		}
		if(m_bSkillFrameShow)
		{
			m_SkillFrame.OnUpdate(dt);
		}
		if(m_SkillFrame.IsNextFrameRestart())
		{
			m_bSkillFrameShow = false;
		}	
	}else
	{
		m_UpdateDelta += dt;
		if (m_UpdateDelta >= dt) {
			m_UpdateDelta = 0;
			if (m_IsMove) {
				if (!m_MoveList.empty())
				{
					double localVelocity = m_MoveVelocity*dt;
					Pos d = m_MoveList.front();
					Pos dest;
					dest.x = d.x * 20 + 10;
					dest.y = d.y * 20 + 10;
					
					Logger::Print("Src X:%.1lf Y:%.1f \t Dest X:%.1lf Y:%.1lf vel:%.1lf dir:%d\n",m_Pos.x,m_Pos.y, dest.x,dest.y,localVelocity,m_Dir);

					if (GMath::Astar_GetDistanceSquare(m_Pos.x, m_Pos.y, dest.x, dest.y) > localVelocity*localVelocity) {
						int degree = GMath::Astar_GetAngle(m_Pos.x, m_Pos.y, dest.x, dest.y);
						m_Dir = GMath::Astar_GetDir(degree);

						double stepRangeX = cos(DegreeToRadian(degree));
						double stepRangeY = sin(DegreeToRadian(degree));

						TranslateX(stepRangeX * localVelocity);
						TranslateY(stepRangeY * localVelocity);
					
						SetDir(m_Dir);
					}
					else {
						Pos d = m_MoveList.front();

						SetX(d.x * 20 + 10);
						SetY(d.y * 20 + 10);
						m_Box.x = d.x;
						m_Box.y = d.y;
						m_MoveList.pop_front();
					//  HandleMoveToCalled();

					}
				}
				else
				{
					m_IsMove = false;
					SetActionID(Player::Idle);
					//SetDir(m_Dir);
					m_Box.x = GetBoxX();
					m_Box.y = GetBoxY();
				// HandleMoveToCalled();

				}
				//Logger::Print("boxX:%d boxY:%d x:%.1lf y:%.1lf dir:%d \n",GetBoxX(),GetBoxY(), GetX(),GetY(), m_Dir);
			}

		}


		if(m_PlayerFrames.find(m_ActionID)!= m_PlayerFrames.end() )
		{
			m_PlayerFrames[m_ActionID].OnUpdate(dt);
			if(m_WeaponFrames.find(m_ActionID)!= m_WeaponFrames.end() )
			{
				m_WeaponFrames[m_ActionID].OnUpdate(dt);
			}
		}
		
		HandleMoveToCalled();
	}
}

void Player::HandleMoveToCalled()
{
    if(m_MoveToCalled)
    {
        if(!m_BackupMoveList.empty())
        {
            m_MoveList=m_BackupMoveList;
            m_IsMove=true;
            Pos d = m_MoveList.front();

            SetX(d.x * 20 + 10);
            SetY(d.y * 20 + 10);
            SetActionID(Player::Moving);
        }
        else
        {
            m_MoveList.clear();
            m_IsMove=false;
            Pos d(GetBoxX(),GetBoxY());

            SetX(d.x * 20 + 10);
            SetY(d.y * 20 + 10);
            SetActionID(Player::Idle);
        }
        m_MoveToCalled=false;
    }
    
}

void Player::OnDraw(int px,int py)
{	
	SpriteRenderer* renderer = SPRITE_RENDERER_INSTANCE;
	if(m_PlayerFrames.find(m_ActionID)!= m_PlayerFrames.end() )
	{
		auto& player = m_PlayerFrames[m_ActionID];
		int _px = px - player.GetWidth() / 2 + 10;
		int _py = py - player.GetHeight() + 20;
        player.SetPos({_px,_py});
		player.Draw();
		
		if(m_WeaponFrames.find(m_ActionID)!= m_WeaponFrames.end() )
		{
			auto& weapon = m_WeaponFrames[m_ActionID];
			int px2 = _px - (weapon.GetKeyX() - player.GetKeyX());
			int py2 = _py - (weapon.GetKeyY() - player.GetKeyY());
            weapon.SetPos({px2,py2});
			weapon.Draw();
		}

		if(!m_NickName.empty())
		{
			auto green = glm::vec3(115/255.0f,1.0f,137/255.0f);
			TextRenderer::GetInstance()->RenderText(m_NickName,px - m_NickName.length()*1.8,SCREEN_HEIGHT-py-32,0.5f,green);
		}
		if(m_bSkillFrameShow)
		{
			int px2 = _px - (m_SkillFrame.GetKeyX() - player.GetKeyX());
			int py2 = _py - (m_SkillFrame.GetKeyY() - player.GetKeyY()) - 15;
            m_SkillFrame.SetPos({px2,py2});
			m_SkillFrame.Draw();
		}
	}
}


void Player::SetPos(double x, double y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

void Player::SetBox()
{
    m_Box.x= GetBoxX();
    m_Box.y = GetBoxY();
}

void Player::MoveTo(GameMap* gameMapPtr, int destBoxX, int destBoxY)
{
	if(m_MoveToCalled)return;
	m_BackupMoveList.clear();
	m_BackupMoveList= gameMapPtr->Move(GetBoxX(), GetBoxY()  , destBoxX, destBoxY);
    m_MoveToCalled=true;
}

void Player::ResetDirAll(int dir)
{
	for (auto& playerIt : m_PlayerFrames)
		playerIt.second.ResetAnim(dir);

	for (auto& weaponIt : m_WeaponFrames)
		weaponIt.second.ResetAnim(dir);
	m_Dir = dir;
}

void Player::ResetDir(int dir)
{
	for (auto& playerIt : m_PlayerFrames)
		playerIt.second.Reset(dir);

	for (auto& weaponIt : m_WeaponFrames)
		weaponIt.second.Reset(dir);	
	// m_Dir = dir;
}

void Player::SetDir(int dir)
{
	for (auto& playerIt : m_PlayerFrames)
		playerIt.second.SetCurrentGroup(dir);

	for (auto& weaponIt : m_WeaponFrames)
		weaponIt.second.SetCurrentGroup(dir);	

	// m_Dir = dir;
}


bool Player::HandleMessage(const Telegram& msg) 
{	
	
	return GetFSM()->HandleMessage(msg);
};

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
	double dist_sqr = GMath::Astar_GetDistanceSquare(player->m_CombatPos.x, player->m_CombatPos.y, player->m_CombatTargetPos.x, player->m_CombatTargetPos.y) ;
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
	//double time_base = 1.0/60*4;
	if (GMath::Astar_GetDistanceSquare(player->m_CombatPos.x, player->m_CombatPos.y, player->m_CombatTargetPos.x, player->m_CombatTargetPos.y) > localVelocity*localVelocity) {
		int degree = GMath::Astar_GetAngle(player->m_CombatPos.x, player->m_CombatPos.y, player->m_CombatTargetPos.x, player->m_CombatTargetPos.y);
		//player->m_Dir = GMath::Astar_GetDir(degree);
		double stepRangeX = cos(DegreeToRadian(degree));
		double stepRangeY = sin(DegreeToRadian(degree));
		player->m_CombatPos.x += stepRangeX*localVelocity;
		player->m_CombatPos.y += stepRangeY*localVelocity;
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
	if(player->m_PlayerFrames.find(6)!= player->m_PlayerFrames.end() )
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
	double dist_sqr = GMath::Astar_GetDistanceSquare(player->m_CombatPos.x, player->m_CombatPos.y, player->m_CombatTargetPos.x, player->m_CombatTargetPos.y) ;
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
	if (GMath::Astar_GetDistanceSquare(player->m_CombatPos.x, player->m_CombatPos.y, player->m_CombatTargetPos.x, player->m_CombatTargetPos.y) > localVelocity*localVelocity) {
		int degree = GMath::Astar_GetAngle(player->m_CombatPos.x, player->m_CombatPos.y, player->m_CombatTargetPos.x, player->m_CombatTargetPos.y);
		//player->m_Dir = GMath::Astar_GetDir(degree);
		double stepRangeX = cos(DegreeToRadian(degree));
		double stepRangeY = sin(DegreeToRadian(degree));
		player->m_CombatPos.x += stepRangeX*localVelocity;
		player->m_CombatPos.y += stepRangeY*localVelocity;
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
	if(player->m_PlayerFrames.find(11)!= player->m_PlayerFrames.end() )
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
	if(player->m_PlayerFrames.find(11)!= player->m_PlayerFrames.end() )
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
	if(player->m_PlayerFrames.find(7)!= player->m_PlayerFrames.end() )
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
