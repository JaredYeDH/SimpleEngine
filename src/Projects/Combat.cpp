#include "Combat.h"
#include "../Engine.h"
#include "Demo.h"
#include "../global.h"
#include "../Message.h"
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

	InputManager::GetInstance()->RegisterOnKeyClickEvent(GLFW_KEY_7 ,
		[this](){
			// for(auto* self: m_Ourselves)
			// {
			// 	if(self!=nullptr)
			// 	{
			// 		self->ChangeAction();
			// 	}
			// }
			//4->11->6->13->4
			static int ac[4] = {4,11,6,13};
			static int index=0;
			auto* self = m_Ourselves[7];
			if(index == 1)
			{
				float x = 220.0f / 640 * SCREEN_WIDTH +70;
				float y = 210.0f / 480 * SCREEN_HEIGHT+70;
                self->SetCombatTargetPos({x,y});
				// self->MoveTo(ac[index++],);	
			}
			else if(index==3)
			{
				float x = 525.0f / 640 * SCREEN_WIDTH ;
				float y = 285.0f / 480 * SCREEN_HEIGHT;
                self->SetCombatTargetPos({x,y});
			}
			self->ChangeAction(ac[index++]);

			index = index==4?0:index;
		}
	);

	InputManager::GetInstance()->RegisterOnKeyClickEvent(GLFW_KEY_2,
		[this](){
			// for(auto* self: m_Ourselves)
			// {
			// 	if(self!=nullptr)
			// 	{
			// 		self->ChangeRole();
			// 	}
			// }
		}
	);

	InputManager::GetInstance()->RegisterOnKeyClickEvent(GLFW_KEY_3,
		[this](){
			// for(auto* self: m_Ourselves)
			// {
			// 	if(self!=nullptr)
			// 	{
			// 		self->ChangeWeapon();
			// 	}
			// }
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
