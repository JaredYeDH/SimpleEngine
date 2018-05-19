#include "Scene.h"

#include "../animation/FrameAnimation.h"
#include "Logger.h"
#include "Random.h"
#include "../core/Renderer.h"
#include "../global.h"
#include "../combat/Combat.h"

static Player * m_StriderPtr = nullptr;
bool s_IsCombat = false;

Scene::Scene(int id,String name)
:BaseScene::BaseScene(id,name)
{
}

Scene::~Scene()
{    
}

void Scene::Init()
{
	m_GameMapPtr = new GameMap(m_ID);

	m_StriderPtr = new Player(3);
	m_StriderPtr->SetPos(500, 500);
	m_StriderPtr->SetBox();
	m_StriderPtr->SetNickName(L"Ocean\u85cf\u5fc3");	//藏心
	m_StriderPtr->SetActionID(15);



	if (s_IsCombat) 
	{
		COMBAT_SYSTEM_INSTANCE;

		
	}
	else 
	{
		INPUT_MANAGER_INSTANCE->RegisterOnKeyClickEvent(GLFW_MOUSE_BUTTON_LEFT, [this](){
			int halfScreenWidth = SCREEN_WIDTH / 2;
			int halfScreenHeight = SCREEN_HEIGHT / 2;

			int mapOffsetX = halfScreenWidth - m_StriderPtr->GetX();
			int mapOffsetY = halfScreenHeight - m_StriderPtr->GetY();

			mapOffsetX = GMath::Clamp(mapOffsetX, -m_GameMapPtr->GetWidth() + SCREEN_WIDTH, 0);
			mapOffsetY = GMath::Clamp(mapOffsetY, -m_GameMapPtr->GetHeight() + SCREEN_HEIGHT, 0);

			double mouseX = INPUT_MANAGER_INSTANCE->GetMouseX();
			double mouseY = INPUT_MANAGER_INSTANCE->GetMouseY();

			IntPos src, dest;
			src.x = m_StriderPtr->GetX();
			src.y = m_StriderPtr->GetY();
			dest.x = -mapOffsetX + mouseX;
			dest.y = -mapOffsetY + mouseY;

			m_StriderPtr->MoveTo(m_GameMapPtr, (-mapOffsetX + mouseX) / 20, (-mapOffsetY + mouseY) / 20);
		});
	}
}

void Scene::Update()
{
	ProcessInput();

	if (s_IsCombat)
	{
		COMBAT_SYSTEM_INSTANCE->Update();
	}
	else
	{
		double dt = ENGINE_INSTANCE->GetDeltaTime();
		m_StriderPtr->OnUpdate(dt);
		if (m_IsChangeState)
		{
			m_ChangeStateTimeInterval += dt;
			if (m_ChangeStateTimeInterval >= 20 * dt)
			{
				m_IsChangeState = false;
				m_ChangeStateTimeInterval = 0;
			}
		}
	}

}

int amout = 1;
void Scene::ProcessInput()
{
	if(INPUT_MANAGER_INSTANCE->IsKeyUp(GLFW_KEY_W))
	{
		m_StriderPtr->TranslateY(-amout);
	};

	if(INPUT_MANAGER_INSTANCE->IsKeyUp(GLFW_KEY_A))
	{
		m_StriderPtr->TranslateX(-amout);
	};


	if(INPUT_MANAGER_INSTANCE->IsKeyUp(GLFW_KEY_S))
	{
		m_StriderPtr->TranslateY(amout);
	};


	if(INPUT_MANAGER_INSTANCE->IsKeyUp(GLFW_KEY_D))
	{
		m_StriderPtr->TranslateX(amout);
	};

	INPUT_MANAGER_INSTANCE->RegisterOnKeyClickEvent(GLFW_KEY_F,[this]()
	{
		s_IsCombat=!s_IsCombat;
	});

	INPUT_MANAGER_INSTANCE->RegisterOnKeyClickEvent(GLFW_KEY_1,
		[this]() {
		m_StriderPtr->ChangeAction();
	}
	);

	INPUT_MANAGER_INSTANCE->RegisterOnKeyClickEvent(GLFW_KEY_2,
		[this]() {
		m_StriderPtr->ChangeRole();

	}
	);

	INPUT_MANAGER_INSTANCE->RegisterOnKeyClickEvent(GLFW_KEY_3,
		[this]() {
		m_StriderPtr->ChangeWeapon();
	}
	);
}

void Scene::Draw()
{
	m_GameMapPtr->Draw(m_StriderPtr->GetX(), m_StriderPtr->GetY());
	

	if (s_IsCombat)
	{
		COMBAT_SYSTEM_INSTANCE->Draw();
	}
	else
	{
		int screenWidth = SCREEN_WIDTH;
		int screenHeight = SCREEN_HEIGHT;
		int halfScreenWidth = screenWidth / 2;
		int halfScreenHeight = screenHeight / 2;
		int mapOffsetX = halfScreenWidth - m_StriderPtr->GetX();
		int mapOffsetY = halfScreenHeight - m_StriderPtr->GetY();
		int mapWidth = m_GameMapPtr->GetWidth();
		int mapHeight = m_GameMapPtr->GetHeight();

		int px = m_StriderPtr->GetX();
		int py = m_StriderPtr->GetY();

		mapOffsetX = GMath::Clamp(mapOffsetX, -mapWidth + screenWidth, 0);
		mapOffsetY = GMath::Clamp(mapOffsetY, -mapHeight + screenHeight, 0);

		int maxMapOffsetX = mapWidth - halfScreenWidth;
		int maxMapOffsetY = mapHeight - halfScreenHeight;

		px = px < halfScreenWidth ? px :
			(px  > maxMapOffsetX ?
			(screenWidth - (mapWidth - px)) : halfScreenWidth);
		py = py < halfScreenHeight ? py :
			(py> maxMapOffsetY ?
			(screenHeight - (mapHeight - py)) : halfScreenHeight);


		//m_GameMapPtr->DrawCell( mapOffsetX, mapOffsetY);
		m_StriderPtr->OnDraw(px, py);


		m_GameMapPtr->DrawMask(m_StriderPtr->GetX(), m_StriderPtr->GetY());
	}
}
