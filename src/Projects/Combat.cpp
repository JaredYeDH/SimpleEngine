#include "Combat.h"
#include "../Engine.h"
#include "Demo.h"
#include "../global.h"
#include "../Message.h"


SkillManager::SkillManager()
{
	m_RandomID=-1;
	utils::tsv skillTSV(Environment::GetTsvPath("magic"));
	m_Skills.clear();
	for(int i=0;i<skillTSV.rows.size()-1;i++)
	{	
		auto row = skillTSV.cols[i];
		int id = std::stoi(row[0]);
		auto _was_id = row[2];
		uint32 was_id= std::stoul(_was_id, 0, 16);

        std::shared_ptr<Sprite2> sp = ResourceManager::GetInstance()->LoadWASSprite(ResourceManager::PackFile::MagicWDF, was_id);
		if(!sp->Error)
		{
			m_Skills[id] = FrameAnimation(sp);
		}
	}
}



CombatSystem::CombatSystem()
: m_Ourselves(10,nullptr),
m_Enemies(10,nullptr)
{
	SKILL_MANAGER_INSTANCE;
}

CombatSystem::~CombatSystem()
{

}

void CombatSystem::AddEnemy(int pos,Player* enemy)
{
	int dir  = static_cast<int>(FrameAnimation::Dir::S_E);
	enemy->ResetDirAll(dir);
	enemy->SetID(10+pos);
	GAME_ENTITY_MANAGER_INSTANCE->RegisterEntity(enemy);
	m_Enemies[pos] = enemy;

}

void CombatSystem::AddSelf(int pos,Player* self)
{
	int dir = static_cast<int>(FrameAnimation::Dir::N_W);
	self->ResetDirAll(dir);
	self->SetID(pos);
	GAME_ENTITY_MANAGER_INSTANCE->RegisterEntity(self);
	m_Ourselves[pos] = self;
}

void CombatSystem::Update()
{
	ProcessInput();

	
	InputManager::GetInstance()->RegisterOnKeyClickEvent(GLFW_KEY_1 ,
			[this](){
				int selfID = RANDOM_INSTANCE->NextInt(0,9);
				int enemyID = RANDOM_INSTANCE->NextInt(0,9);
				auto& self = m_Ourselves[selfID];
				auto& enemy = m_Enemies[enemyID];
				// double dist_sqr = GMath::Astar_GetDistanceSquare(self->GetCombatPos().x, self->GetCombatPos().y, 
				// enemy->GetCombatPos().x, enemy->GetCombatPos().y) ;
				// double d = std::sqrt(dist_sqr);
				int degree = GMath::Astar_GetAngle(self->GetCombatPos().x, self->GetCombatPos().y, 
				enemy->GetCombatPos().x + 90 , enemy->GetCombatPos().y + 60);
				int dir = GMath::Astar_GetDir(degree);	
				double stepRangeX = cos(DegreeToRadian(degree)) * 90;
				double stepRangeY = sin(DegreeToRadian(degree)) * 90;

				float x = enemy->GetCombatPos().x - stepRangeX;
				float y = enemy->GetCombatPos().y - stepRangeY;
    			self->SetCombatTargetPos({enemy->GetCombatPos().x + 88 , enemy->GetCombatPos().y + 73});
				self->SetTargetID(enemyID+10);
				self->GetFSM()->ChangeState(PlayerCombatMoveState::GetInstance());
			}
		);

	InputManager::GetInstance()->RegisterOnKeyClickEvent(GLFW_KEY_2 ,
		[this](){
			int selfID = RANDOM_INSTANCE->NextInt(0,9);
			auto& self = m_Ourselves[selfID];
			self->GetFSM()->ChangeState(PlayerCombatCastAttackState::GetInstance());
		}
	);	
	
	MESSAGE_DISPATCHER_INSTANCE->DispatchDelayedMessage();

	double dt = ENGINE_INSTANCE->GetDeltaTime(); 
	for(auto* self: m_Ourselves)
	{
		if(self!=nullptr)
		{
			self->OnUpdate(dt);
		}
	}

	for(auto* enemy: m_Enemies)
	{
		if(enemy!=nullptr)
		{
			enemy->OnUpdate(dt);
		}
	}
	
}

void CombatSystem::Draw()
{
	

	for(auto* enemy: m_Enemies)
	{
		if(enemy!=nullptr)
		{
			Pos pos = enemy->GetCombatPos();
			enemy->OnDraw(pos.x,pos.y);
		}
	}

    for(auto* self: m_Ourselves)
	{
		if(self!=nullptr)
		{
			Pos pos = self->GetCombatPos();
			self->OnDraw(pos.x,pos.y);
		}
	}
	//m_OtherPtr->OnDraw(m_RendererPtr, m_OtherPtr->GetX() + mapOffsetX,m_OtherPtr->GetY() + mapOffsetY);
}

void CombatSystem::ProcessInput()
{
	InputManager::GetInstance()->RegisterOnKeyClickEvent(GLFW_KEY_3,
		[this](){
			for(auto* self: m_Ourselves)
			{
				if(self!=nullptr)
				{
					self->ChangeRole();
				}
			}
		}
	);

	InputManager::GetInstance()->RegisterOnKeyClickEvent(GLFW_KEY_4 ,
		[this](){
			for(auto* self: m_Ourselves)
			{
				if(self!=nullptr)
				{
					self->ChangeWeapon();
				}
			}
		}
	);


	InputManager::GetInstance()->RegisterOnKeyClickEvent(GLFW_KEY_5,
		[this](){
			for(auto* enemy: m_Enemies)
			{
				if(enemy!=nullptr)
				{
					enemy->ChangeAction();
				}
			}
		}
	);

	InputManager::GetInstance()->RegisterOnKeyClickEvent(GLFW_KEY_6,
		[this](){
			for(auto* enemy: m_Enemies)
			{
				if(enemy!=nullptr)
				{
					enemy->ChangeRole();
				}
			}
		}
	);

	InputManager::GetInstance()->RegisterOnKeyClickEvent(GLFW_KEY_7,
		[this](){
			for(auto* enemy: m_Enemies)
			{
				if(enemy!=nullptr)
				{
					enemy->ChangeWeapon();
				}
			}
		}
	);
}
