#include "Combat.h"
#include "../Engine.h"
#include "Demo.h"
CombatSystem::CombatSystem()
{
	
	
}

CombatSystem::~CombatSystem()
{

}



void CombatSystem::Init(Player* self,Player* enemy)
{
	m_Self = self;
	m_Enemy = enemy;

	
};

void CombatSystem::Update()
{
	double dt = Engine::GetInstance()->GetDeltaTime(); 
	m_Self->OnUpdate(dt);
	m_Enemy->OnUpdate(dt);
}


void CombatSystem::Draw()
{
	int screenWidth = Demo::GetScreenWidth();
	int screenHeight = Demo::GetScreenHeight();
	int halfScreenWidth = screenWidth / 2;
	int halfScreenHeight = screenHeight / 2;

		
	int px = halfScreenWidth;
	int py = halfScreenHeight;

    m_Self->OnDraw(SpriteRenderer::GetInstance(),px,py);
	
	//m_OtherPtr->OnDraw(m_RendererPtr, m_OtherPtr->GetX() + mapOffsetX,m_OtherPtr->GetY() + mapOffsetY);	

}
