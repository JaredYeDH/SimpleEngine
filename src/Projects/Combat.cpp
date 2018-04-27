#include "Combat.h"
#include "../Engine.h"
#include "Demo.h"

CombatSystem::CombatSystem()
: m_Ourselves(10,nullptr),
m_Enemies(10,nullptr)
{
	
}

CombatSystem::~CombatSystem()
{

}

void CombatSystem::AddEnemy(int pos,Player* enemy)
{
	int dir  = static_cast<int>(FrameAnimation::Dir::S_E);
	enemy->ResetDirAll(dir);
	m_Enemies[pos] = enemy;
}

void CombatSystem::AddSelf(int pos,Player* self)
{
	int dir = static_cast<int>(FrameAnimation::Dir::N_W);
	self->ResetDirAll(dir);
	m_Ourselves[pos] = self;
}

void CombatSystem::Update()
{
	double dt = Engine::GetInstance()->GetDeltaTime(); 
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

	ProcessInput();
}

void CombatSystem::Draw()
{
	for(auto* self: m_Ourselves)
	{
		if(self!=nullptr)
		{
			Pos pos = self->GetCombatPos();
			self->OnDraw(SpriteRenderer::GetInstance(),pos.x,pos.y);
		}
	}

	for(auto* enemy: m_Enemies)
	{
		if(enemy!=nullptr)
		{
			Pos pos = enemy->GetCombatPos();
			enemy->OnDraw(SpriteRenderer::GetInstance(),pos.x,pos.y);
		}
	}

    
	//m_OtherPtr->OnDraw(m_RendererPtr, m_OtherPtr->GetX() + mapOffsetX,m_OtherPtr->GetY() + mapOffsetY);
}

void CombatSystem::ProcessInput()
{
	InputManager::GetInstance()->RegisterOnKeyClickEvent(GLFW_KEY_1,
		[this](){
			for(auto* self: m_Ourselves)
			{
				if(self!=nullptr)
				{
					self->ChangeAction();
				}
			}
		}
	);

	InputManager::GetInstance()->RegisterOnKeyClickEvent(GLFW_KEY_2,
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

	InputManager::GetInstance()->RegisterOnKeyClickEvent(GLFW_KEY_3,
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

	InputManager::GetInstance()->RegisterOnKeyClickEvent(GLFW_KEY_4,
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

	InputManager::GetInstance()->RegisterOnKeyClickEvent(GLFW_KEY_5,
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

	InputManager::GetInstance()->RegisterOnKeyClickEvent(GLFW_KEY_6,
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
