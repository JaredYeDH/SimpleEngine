#include "Demo.h"

#include "FrameAnimation.h"
#include "Logger.h"
#include "Random.h"
#include <asio.hpp>
#include <thread>
#include "../core/Renderer.h"
#include "../global.h"
#include "../combat/Combat.h"
#include "../Message.h"

float Demo::s_ScreenWidth = SCREEN_WIDTH;
float Demo::s_ScreenHeight = SCREEN_HEIGHT;

void Demo::OnEvent(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		int halfScreenWidth = GetScreenWidth() / 2;
		int halfScreenHeight = GetScreenHeight() / 2;

		int mapOffsetX = halfScreenWidth - m_StriderPtr->GetX();
		int mapOffsetY = halfScreenHeight - m_StriderPtr->GetY();

		mapOffsetX = GMath::Clamp(mapOffsetX, -m_GameMapPtr->GetWidth() + s_ScreenWidth, 0);
		mapOffsetY = GMath::Clamp(mapOffsetY, -m_GameMapPtr->GetHeight() + s_ScreenHeight, 0);

		double mouseX = InputManager::GetInstance()->GetMouseX();
		double mouseY = InputManager::GetInstance()->GetMouseY();

		IntPos src, dest;
		src.x = m_StriderPtr->GetX();
		src.y = m_StriderPtr->GetY();
		dest.x = -mapOffsetX + mouseX;
		dest.y = -mapOffsetY + mouseY;

		m_StriderPtr->MoveTo(m_GameMapPtr, (-mapOffsetX + mouseX) / 20, (-mapOffsetY + mouseY) / 20);
	}
}

void Demo::OnMove(MoveMessage msg)
{
	if (m_OtherPtr == nullptr || m_StriderPtr == nullptr)return;
	//if (m_StriderPtr->IsMove())return;
	m_OtherPtr->SetPos(msg.m_Src.x, msg.m_Src.y);
	m_OtherPtr->SetBox();
	m_OtherPtr->MoveTo(m_GameMapPtr, (msg.m_Dest.x) / 20, (msg.m_Dest.y) / 20);
}

Player* Demo::m_OtherPtr = nullptr;
Player* Demo::m_StriderPtr = nullptr;


// CombatSystem* s_CombatSystem;
bool s_IsCombat = true;
Demo::Demo()
	:m_IsTestNpc0(true)
{

	m_IsTestNpc0 = false;
	InputManager::GetInstance()->SetMouseEvent(this);

	m_GameMapPtr = new GameMap(0);

	auto blockPath = Environment::GetAbsPath("Resource/Assets/wall.jpg");
	m_BlockTexturePtr = new Texture(blockPath);

	m_StriderPtr = new Player(3);
	m_StriderPtr->SetPos(2300, 1700);
	m_StriderPtr->SetBox();
	m_StriderPtr->SetNickName(L"Ocean-\u85cf\u5fc3");	//藏心
	m_StriderPtr->SetActionID(15);

	m_OtherPtr = new Player(4);
	m_OtherPtr->SetPos(990, 650);
	m_OtherPtr->SetBox();

	if (s_IsCombat) {
		std::shared_ptr<Sprite2> sp = ResourceManager::GetInstance()->LoadWASSprite(ResourceManager::AddonWDF, 0x708C11A0);
		FrameAnimation combatBG(sp);
		Renderer2D::GetInstance()->AddObject(new Image(
			combatBG.GetFramePath(0), Vec2(0, 0), Vec2(s_ScreenWidth, s_ScreenHeight))
		);

	}

	//TestServer();
	COMBAT_SYSTEM_INSTANCE;
	// s_CombatSystem = new CombatSystem();


}

Demo::~Demo()
{
	m_ClientPtr->Close();
}

void Demo::Update()
{
	if (s_IsCombat)
	{
		COMBAT_SYSTEM_INSTANCE->Update();

	}
	else
	{
		double dt = ENGINE_INSTANCE->GetDeltaTime();
		m_StriderPtr->OnUpdate(dt);
		m_OtherPtr->OnUpdate(dt);
		for (Player* npc : m_NPCs)
		{
			npc->OnUpdate(dt);
		}

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

void Demo::ProcessInput()
{
	if (InputManager::GetInstance()->IsKeyUp(GLFW_KEY_T) && m_IsTestNpc0)
	{
		bool movingSuccess = false;
		do
		{
			int destBoxX, destBoxY;
			destBoxX = RANDOM_INSTANCE->NextInt(0, m_GameMapPtr->GetWidth() / 20 - 1);
			destBoxY = RANDOM_INSTANCE->NextInt(0, m_GameMapPtr->GetHeight() / 20 - 1);
			int velocity;
			velocity = RANDOM_INSTANCE->NextInt(150, 375);
			m_NPCs[0]->SetVelocity(velocity);
			m_NPCs[0]->MoveTo(m_GameMapPtr, destBoxX, destBoxY);
			movingSuccess = m_NPCs[0]->IsMove();
		} while (!movingSuccess);
	}


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

ImVec4 clear_color = ImColor(114, 144, 154);
bool show_test_window = true;
bool show_another_window = false;
void Demo::Draw()
{
	m_GameMapPtr->Draw(m_StriderPtr->GetX(), m_StriderPtr->GetY());
	Renderer2D::GetInstance()->Render();

	if (s_IsCombat)
	{
		COMBAT_SYSTEM_INSTANCE->Draw();
	}
	else
	{
		int screenWidth = Demo::GetScreenWidth();
		int screenHeight = Demo::GetScreenHeight();
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

		m_OtherPtr->OnDraw(m_OtherPtr->GetX() + mapOffsetX, m_OtherPtr->GetY() + mapOffsetY);

		m_GameMapPtr->DrawMask(m_StriderPtr->GetX(), m_StriderPtr->GetY());
	}

	/*for (Player* npc : m_NPCs)
	{
	npc->OnDraw( npc->GetX() + mapOffsetX, npc->GetY() + mapOffsetY);
	}*/
	//m_Render.Render();

	//// 1. Show a simple window
	//// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
	//{
	//	static float f = 0.0f;
	//	ImGui::Text("Hello, world!");
	//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	//	ImGui::ColorEdit3("clear color", (float*)&clear_color);
	//	if (ImGui::Button("Test Window")) show_test_window ^= 1;
	//	if (ImGui::Button("Another Window")) show_another_window ^= 1;
	//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//}

	//// 2. Show another simple window, this time using an explicit Begin/End pair
	//if (show_another_window)
	//{
	//	ImGui::Begin("Another Window", &show_another_window);
	//	ImGui::Text("Hello from another window!");
	//	ImGui::End();
	//}

	//// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	//if (show_test_window)
	//{
	//	ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
	//	ImGui::ShowTestWindow(&show_test_window);
	//}

	//ImGui::Render();
}

void Demo::TestServer()
{
	asio::ip::tcp::iostream s("www.boost.org", "http");
	s.expires_from_now(std::chrono::seconds(60));
	s << "GET / HTTP/1.0\r0.2cm\n";
	s << "Host: www.boost.org\r\n";
	s << "Accept: */*\r\n";
	s << "Connection: close\r\n\r\n";
	std::string header;
	while (std::getline(s, header) && header != "\r")
		std::cout << header << "\n";
	std::cout << s.rdbuf();
	if (!s)
	{
		std::cout << "Socket error: " << s.error().message() << "\n";
		return;
	}
}
