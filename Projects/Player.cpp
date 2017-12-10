#include "Player.h"
#include "ResourceManager.h"
#include "Demo.h"
#include "Logger.h"

//
//map 1501.map
//shape.wdf 49386FCE 54F3FC94
//shape.wd3 DF749306 1BEC0D8A

// PlayerId : 1-12 剑侠客是1
// WeaponId : 0-160 对应某个武器

//shape.wdf A16A06FF 4FBA48B8 
//shape.wd3 72013AF5 F2FB1AFA 
std::map<uint32, std::vector< uint32>> Player::s_PlayerAnimationTable =
{
	{ 1, { 0x49386FCE, 0x54F3FC94 } },
	{ 11,{ 0xA16A06FF , 0x4FBA48B8 } }
};

std::map<uint32, std::map<uint32, std::vector< uint32>>> Player::s_WeaponAnimationTable =
{
	{ 1, { { 120, { 0xDF749306, 0x1BEC0D8A } } } },
	{ 11,{ { 120,{ 0x72013AF5, 0xF2FB1AFA } } } }
};


Player::Player(int id ,int PlayerId,int WeaponId):
m_Id(id),
m_PlayerAnimation(2),
m_WeapAnimation(2),
m_AnimationState(Idle),
m_IsMove(false),
m_MoveVelocity(400),
m_UpdateDelta(0),
m_MoveList(),
m_BackupMoveList(),
m_MoveToCalled(false)
{
	m_PlayerAnimation[Idle] = new FrameAnimation(
		ResourceManager::GetInstance()->LoadWdfSprite(s_PlayerAnimationTable[PlayerId][Idle])
		);

	m_PlayerAnimation[Moving] = new FrameAnimation(
		ResourceManager::GetInstance()->LoadWdfSprite(s_PlayerAnimationTable[PlayerId][Moving])
		) ;

	m_WeapAnimation[Idle] = new FrameAnimation(
		ResourceManager::GetInstance()->LoadWd3Sprite(s_WeaponAnimationTable[PlayerId][WeaponId][Idle])
		);

	m_WeapAnimation[Moving] = new FrameAnimation(
		ResourceManager::GetInstance()->LoadWd3Sprite(s_WeaponAnimationTable[PlayerId][WeaponId][Moving])
		);
}

Player::~Player()
{

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

				if (GMath::Astar_GetDistanceSquare(m_Pos.x, m_Pos.y, dest.x, dest.y) > localVelocity*localVelocity) {
					int degree = GMath::Astar_GetAngle(m_Box.x, m_Box.y, d.x, d.y);

					m_Dir = GMath::Astar_GetDir(degree);

					//Logger::Print("degree:%lf dir:%d \n", degree, m_Dir);

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
				SetAnimationState(Player::Idle);
				//SetDir(m_Dir);
				m_Box.x = GetBoxX();
				m_Box.y = GetBoxY();
             // HandleMoveToCalled();

			}
			//Logger::Print("cur_x:%lf cur_y:%lf\n",GetX(), GetY());
		}

	}

	m_PlayerAnimation[m_AnimationState]->OnUpdate(dt);
	m_WeapAnimation[m_AnimationState]->OnUpdate(dt);
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
            SetAnimationState(Player::Moving);
        }
        else
        {
            m_MoveList.clear();
            m_IsMove=false;
            Pos d(GetBoxX(),GetBoxY());

            SetX(d.x * 20 + 10);
            SetY(d.y * 20 + 10);
            SetAnimationState(Player::Idle);
        }
        m_MoveToCalled=false;
    }
    
}


void Player::OnDraw(SpriteRenderer * renderer, int px,int py)
{
	px = px - m_PlayerAnimation[m_AnimationState]->GetWidth() / 2 + 10;
	py = py - m_PlayerAnimation[m_AnimationState]->GetHeight() + 20;

	m_PlayerAnimation[m_AnimationState]->Draw(renderer, px, py);

	int px2 = px - (m_WeapAnimation[m_AnimationState]->GetKeyX() - m_PlayerAnimation[m_AnimationState]->GetKeyX());
	int py2 = py - (m_WeapAnimation[m_AnimationState]->GetKeyY() - m_PlayerAnimation[m_AnimationState]->GetKeyY());

	m_WeapAnimation[m_AnimationState]->Draw(renderer, px2, py2);
	
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
	m_WeapAnimation[Idle]->Reset(dir);
	m_WeapAnimation[Moving]->Reset(dir);
	m_PlayerAnimation[Idle]->Reset(dir);
	m_PlayerAnimation[Moving]->Reset(dir);
}

void Player::ResetDir(int dir)
{
    m_WeapAnimation[Idle]->Reset(dir);
    m_WeapAnimation[Moving]->Reset(dir);
    m_PlayerAnimation[Idle]->Reset(dir);
    m_PlayerAnimation[Moving]->Reset(dir);
}

void Player::SetDir(int dir)
{
    m_WeapAnimation[Idle]->SetCurrentGroup(dir);
    m_WeapAnimation[Moving]->SetCurrentGroup(dir);
    m_PlayerAnimation[Idle]->SetCurrentGroup(dir);
    m_PlayerAnimation[Moving]->SetCurrentGroup(dir);
}


