#include "Combat.h"
#include "../Engine.h"
#include "../global.h"
#include "../Message.h"
#include "../simple_engine.h"
#include "../animation/FrameAnimation.h"
#include "Skill.h"
#include "../actor/PlayerState.h"
#include "../core/Renderer.h"

static	FrameAnimation* s_CombatBG;
CombatSystem::CombatSystem()
: m_Ourselves(10,nullptr),
m_Enemies(10,nullptr)
{
	SKILL_MANAGER_INSTANCE;
	auto f = [](int role_id, double x, double y, std::wstring nickname)
	{
		Player* p = new Player(role_id);
		p->SetActionID(4);
		p->SetPos(x, y);
		p->SetCombatPos(x, y);
		p->SetCombatTargetPos({ x*1.0f,y*1.0f });
		p->SetIsCombat(true);
		p->SetNickName(nickname);
		p->ChangeWeapon();
		return p;
	};
	AddSelf(0, f(3, 415.0f / 640 * SCREEN_WIDTH, 275.0f / 480 * SCREEN_HEIGHT, L"\u5df1\u65b9\u7ec40")); //己方组
	AddSelf(1, f(3, 355.0f / 640 * SCREEN_WIDTH, 305.0f / 480 * SCREEN_HEIGHT, L"\u5df1\u65b9\u7ec401"));
	AddSelf(2, f(3, 475.0f / 640 * SCREEN_WIDTH, 245.0f / 480 * SCREEN_HEIGHT, L"\u5df1\u65b9\u7ec402"));
	AddSelf(3, f(3, 295.0f / 640 * SCREEN_WIDTH, 335.0f / 480 * SCREEN_HEIGHT, L"\u5df1\u65b9\u7ec403"));
	AddSelf(4, f(3, 535.0f / 640 * SCREEN_WIDTH, 215.0f / 480 * SCREEN_HEIGHT, L"\u5df1\u65b9\u7ec404"));
	AddSelf(5, f(3, 465.0f / 640 * SCREEN_WIDTH, 315.0f / 480 * SCREEN_HEIGHT, L"\u5df1\u65b9\u7ec405"));
	AddSelf(6, f(3, 405.0f / 640 * SCREEN_WIDTH, 345.0f / 480 * SCREEN_HEIGHT, L"\u5df1\u65b9\u7ec406"));
	AddSelf(7, f(3, 525.0f / 640 * SCREEN_WIDTH, 285.0f / 480 * SCREEN_HEIGHT, L"\u5df1\u65b9\u7ec407"));
	AddSelf(8, f(3, 345.0f / 640 * SCREEN_WIDTH, 375.0f / 480 * SCREEN_HEIGHT, L"\u5df1\u65b9\u7ec408"));
	AddSelf(9, f(3, 585.0f / 640 * SCREEN_WIDTH, 255.0f / 480 * SCREEN_HEIGHT, L"\u5df1\u65b9\u7ec409"));

	AddEnemy(0, f(4, 175.0f / 640 * SCREEN_WIDTH, 170.0f / 480 * SCREEN_HEIGHT, L"\u654c\u65b9\u7ec40"));	//敌方组
	AddEnemy(1, f(4, 115.0f / 640 * SCREEN_WIDTH, 200.0f / 480 * SCREEN_HEIGHT, L"\u654c\u65b9\u7ec401"));
	AddEnemy(2, f(4, 235.0f / 640 * SCREEN_WIDTH, 140.0f / 480 * SCREEN_HEIGHT, L"\u654c\u65b9\u7ec402"));
	AddEnemy(3, f(4, 55.0f / 640 * SCREEN_WIDTH, 230.0f / 480 * SCREEN_HEIGHT, L"\u654c\u65b9\u7ec403"));
	AddEnemy(4, f(4, 295.0f / 640 * SCREEN_WIDTH, 110.0f / 480 * SCREEN_HEIGHT, L"\u654c\u65b9\u7ec404"));
	AddEnemy(5, f(4, 220.0f / 640 * SCREEN_WIDTH, 210.0f / 480 * SCREEN_HEIGHT, L"\u654c\u65b9\u7ec405"));
	AddEnemy(6, f(4, 160.0f / 640 * SCREEN_WIDTH, 240.0f / 480 * SCREEN_HEIGHT, L"\u654c\u65b9\u7ec406"));
	AddEnemy(7, f(4, 280.0f / 640 * SCREEN_WIDTH, 180.0f / 480 * SCREEN_HEIGHT, L"\u654c\u65b9\u7ec407"));
	AddEnemy(8, f(4, 100.0f / 640 * SCREEN_WIDTH, 270.0f / 480 * SCREEN_HEIGHT, L"\u654c\u65b9\u7ec408"));
	AddEnemy(9, f(4, 340.0f / 640 * SCREEN_WIDTH, 150.0f / 480 * SCREEN_HEIGHT, L"\u654c\u65b9\u7ec409"));

	Sprite* sp = ResourceManager::GetInstance()->LoadWASSprite(ResourceManager::ADDONWDF, 0x708C11A0);
	
	s_CombatBG = new FrameAnimation(sp);

	// RENDERER_2D_INSTANCE->AddObject(new Image(
	// 	s_CombatBG->GetFramePath(0), Vec2(0, 0), Vec2(SCREEN_WIDTH, SCREEN_HEIGHT))
	// );
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
	s_CombatBG->OnUpdate();
	ProcessInput();

	InputManager::GetInstance()->RegisterOnKeyClickEvent(GLFW_KEY_1 ,
			[this](){
				int selfID = RANDOM_INSTANCE->NextInt(0,9);
				int enemyID = RANDOM_INSTANCE->NextInt(0,9);
				auto& self = m_Ourselves[selfID];
				auto& enemy = m_Enemies[enemyID];
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
	s_CombatBG->Draw();
	//RENDERER_2D_INSTANCE->Render();

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
