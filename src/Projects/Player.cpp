#include "Player.h"
#include "ResourceManager.h"
#include "Demo.h"
#include "Logger.h"
#include "global.h"
//
//map 1501.map
//shape.wdf 49386FCE 54F3FC94
//shape.wd3 DF749306 1BEC0D8A

// PlayerId : 1-12 
// WeaponId : 0-160

//shape.wdf A16A06FF 4FBA48B8 
//shape.wd3 72013AF5 F2FB1AFA


Player::Player(int roleID):
m_ID(-1),
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
m_AnimDB()
{
	LogInfo();
	
	
}


Player::Player()
:Player(0)
{
	m_RoleID = 3;
	m_WeaponID = 7;
	m_ActionID = Player::Idle;
	

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
	m_RoleID = m_RoleID == 3 ? 4: 3;
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
				m_WeaponFrames[actionID].ResetFrameTime(m_PlayerFrames[actionID].GetGroupFrameCount());
			}
		}
	}
}

void Player::ChangeWeapon()
{
	m_WeaponID = m_RoleID == 3 ? 7 : 3;
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
				m_WeaponFrames[actionID].ResetFrameTime(m_PlayerFrames[actionID].GetGroupFrameCount());
			}
		}
	}

	
	
}

void Player::ChangeAction()
{
	m_ActionID++;
	if(m_ActionID > 20)m_ActionID = 1;
	
}

void Player::SaveFrame(int index)
{
	ResourceManager::GetInstance()->SaveWdfSprite(0x49386FCE);
}

void Player::OnUpdate(double dt)
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


void Player::OnDraw(SpriteRenderer * renderer, int px,int py)
{	
	if(m_PlayerFrames.find(m_ActionID)!= m_PlayerFrames.end() )
	{
		auto& player = m_PlayerFrames[m_ActionID];
		int _px = px - player.GetWidth() / 2 + 10;
		int _py = py - player.GetHeight() + 20;
		player.Draw(renderer,_px,_py);
		
		if(m_WeaponFrames.find(m_ActionID)!= m_WeaponFrames.end() )
		{
			auto& weapon = m_WeaponFrames[m_ActionID];
			int px2 = _px - (weapon.GetKeyX() - player.GetKeyX());
			int py2 = _py - (weapon.GetKeyY() - player.GetKeyY());

			weapon.Draw(renderer,px2,py2);
		}

        if(!m_NickName.empty())
		{
			auto green = glm::vec3(115/255.0f,1.0f,137/255.0f);
			TextRenderer::GetInstance()->RenderText(m_NickName,px - m_NickName.length()*1.8,SCREEN_HEIGHT-py-32,0.5f,green);
		}
	}
}

void Player::PlayAction(int action,int dir)
{
	SetActionID(action);
	if(m_PlayerFrames.find(m_ActionID)!= m_PlayerFrames.end() )
	{
		m_PlayerFrames[m_ActionID].ResetAnim(m_Dir);
		if(m_WeaponFrames.find(m_ActionID)!= m_WeaponFrames.end() )
		{
			m_WeaponFrames[m_ActionID].ResetAnim(m_Dir);
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


