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
	m_StriderPtr->SetNickName(L"Ocean-\u85cf\u5fc3");	//藏心
	m_StriderPtr->SetActionID(15);


	if (s_IsCombat) 
	{
		COMBAT_SYSTEM_INSTANCE;

		std::shared_ptr<Sprite2> sp = ResourceManager::GetInstance()->LoadWASSprite(ResourceManager::AddonWDF, 0x708C11A0);
		FrameAnimation combatBG(sp);
		Renderer2D::GetInstance()->AddObject(new Image(
			combatBG.GetFramePath(0), Vec2(0, 0), Vec2(SCREEN_WIDTH, SCREEN_HEIGHT))
		);
	}
	else 
	{
		InputManager::GetInstance()->RegisterOnKeyClickEvent(GLFW_MOUSE_BUTTON_LEFT, [this](){
			int halfScreenWidth = SCREEN_WIDTH / 2;
			int halfScreenHeight = SCREEN_HEIGHT / 2;

			int mapOffsetX = halfScreenWidth - m_StriderPtr->GetX();
			int mapOffsetY = halfScreenHeight - m_StriderPtr->GetY();

			mapOffsetX = GMath::Clamp(mapOffsetX, -m_GameMapPtr->GetWidth() + SCREEN_WIDTH, 0);
			mapOffsetY = GMath::Clamp(mapOffsetY, -m_GameMapPtr->GetHeight() + SCREEN_HEIGHT, 0);

			double mouseX = InputManager::GetInstance()->GetMouseX();
			double mouseY = InputManager::GetInstance()->GetMouseY();

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
	if (s_IsCombat)
	{
		COMBAT_SYSTEM_INSTANCE->Update();
	}
	else
	{
		double dt = ENGINE_INSTANCE->GetDeltaTime();
		m_StriderPtr->OnUpdate(dt);

		ProcessInput();

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

void Scene::ProcessInput()
{

	int amout = 1;
	if (InputManager::GetInstance()->IsKeyUp(GLFW_KEY_W))
	{
		m_StriderPtr->TranslateY(-amout);
		Logger::Print("cur_x:%lf cur_y:%lf\n", m_StriderPtr->GetX(), m_StriderPtr->GetY());
	}

	if (InputManager::GetInstance()->IsKeyUp(GLFW_KEY_A))
	{
		m_StriderPtr->TranslateX(-amout);
		Logger::Print("cur_x:%lf cur_y:%lf\n", m_StriderPtr->GetX(), m_StriderPtr->GetY());
	}


	if (InputManager::GetInstance()->IsKeyUp(GLFW_KEY_S))
	{
		m_StriderPtr->TranslateY(amout);
		Logger::Print("cur_x:%lf cur_y:%lf\n", m_StriderPtr->GetX(), m_StriderPtr->GetY());
	}


	if (InputManager::GetInstance()->IsKeyUp(GLFW_KEY_D))
	{
		m_StriderPtr->TranslateX(amout);
		Logger::Print("cur_x:%lf cur_y:%lf\n", m_StriderPtr->GetX(), m_StriderPtr->GetY());
	}

	int dir = 0;

	if (InputManager::GetInstance()->IsKeyUp(GLFW_KEY_KP_3) || InputManager::GetInstance()->IsKeyUp(GLFW_KEY_3))
	{
		dir = static_cast<int>(FrameAnimation::Dir::S_E);
		m_StriderPtr->SetDir(dir);
	}

	// if (InputManager::GetInstance()->IsKeyUp(GLFW_KEY_KP_1)  || InputManager::GetInstance()->IsKeyUp(GLFW_KEY_1))
	// {
	// 	dir = static_cast<int>(FrameAnimation::Dir::S_W);
	// 	m_StriderPtr->SetDir(dir);
	// }

	if (InputManager::GetInstance()->IsKeyUp(GLFW_KEY_KP_7) || InputManager::GetInstance()->IsKeyUp(GLFW_KEY_7))
	{

		dir = static_cast<int>(FrameAnimation::Dir::N_W);
		m_StriderPtr->SetDir(dir);
	}


	if (InputManager::GetInstance()->IsKeyUp(GLFW_KEY_KP_9) || InputManager::GetInstance()->IsKeyUp(GLFW_KEY_9))
	{
		dir = static_cast<int>(FrameAnimation::Dir::N_E);
		m_StriderPtr->SetDir(dir);
	}


	// if (InputManager::GetInstance()->IsKeyUp(GLFW_KEY_KP_2)  || InputManager::GetInstance()->IsKeyUp(GLFW_KEY_2))
	// {
	// 	dir = static_cast<int>(FrameAnimation::Dir::S);
	// 	m_StriderPtr->SetDir(dir);
	// }


	if (InputManager::GetInstance()->IsKeyUp(GLFW_KEY_KP_4) || InputManager::GetInstance()->IsKeyUp(GLFW_KEY_4))
	{
		dir = static_cast<int>(FrameAnimation::Dir::W);
		m_StriderPtr->SetDir(dir);
	}

	if (InputManager::GetInstance()->IsKeyUp(GLFW_KEY_KP_8) || InputManager::GetInstance()->IsKeyUp(GLFW_KEY_8))
	{

		dir = static_cast<int>(FrameAnimation::Dir::N);
		m_StriderPtr->SetDir(dir);
	}


	if (InputManager::GetInstance()->IsKeyUp(GLFW_KEY_KP_6) || InputManager::GetInstance()->IsKeyUp(GLFW_KEY_6))
	{
		dir = static_cast<int>(FrameAnimation::Dir::E);
		m_StriderPtr->SetDir(dir);
	}



	InputManager::GetInstance()->RegisterOnKeyClickEvent(GLFW_KEY_1,
		[this]() {
		m_StriderPtr->ChangeAction();
	}
	);

	InputManager::GetInstance()->RegisterOnKeyClickEvent(GLFW_KEY_2,
		[this]() {
		m_StriderPtr->ChangeRole();

	}
	);

	InputManager::GetInstance()->RegisterOnKeyClickEvent(GLFW_KEY_3,
		[this]() {
		m_StriderPtr->ChangeWeapon();
	}
	);

	if (InputManager::GetInstance()->IsKeyUp(GLFW_KEY_KP_5) && !m_IsChangeState)
	{
		m_IsChangeState = true;

		if (m_StriderPtr->GetActionID() == Player::Moving)
		{
			m_StriderPtr->SetActionID(Player::Idle);
		}
		else
		{
			m_StriderPtr->SetActionID(Player::Moving);
		}
		m_StriderPtr->ResetDir(dir);
	}

}

void Scene::Draw()
{
	m_GameMapPtr->Draw(m_StriderPtr->GetX(), m_StriderPtr->GetY());
	Renderer2D::GetInstance()->Render();

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
