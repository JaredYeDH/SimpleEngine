#include "Demo.h"

#include "FrameAnimation.h"
#include "Logger.h"
#include "Random.h"
#include <asio.hpp>
#include <thread>

float Demo::s_ScreenWidth = 800.0f;
float Demo::s_ScreenHeight = 600.0f;
bool g_IsTest = false;

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

		MoveMessage msg(Demo::g_Id, src, dest, "hello");
		msg.EncodeBody();
		msg.EncodeHeader();
		m_ClientPtr->Write(msg);

		m_StriderPtr->MoveTo(m_GameMapPtr,(-mapOffsetX + mouseX) / 20,(-mapOffsetY + mouseY) / 20);
	}
}


void Demo::SetClient(Client* clientPtr)
{
	m_ClientPtr = clientPtr;
}


void Demo::OnMove(MoveMessage msg)
{ 
	if (msg.m_Pid == g_Id)return;

	if (m_OtherPtr == nullptr || m_StriderPtr == nullptr)return;

	//if (m_StriderPtr->IsMove())return;

	
	m_OtherPtr->SetPos(msg.m_Src.x,msg.m_Src.y);
	m_OtherPtr->SetBox();
	m_OtherPtr->MoveTo(m_GameMapPtr, (msg.m_Dest.x ) / 20, (msg.m_Dest.y) / 20);
}

Player* Demo::m_OtherPtr  = nullptr;
Player* Demo::m_StriderPtr = nullptr;
Demo::Demo()
	:m_IsTestNpc0(true)
{

    m_IsTestNpc0 =false;
	InputManager::GetInstance()->SetMouseEvent(this);
	
	m_RendererPtr = new SpriteRenderer();

	m_GameMapPtr = new GameMap(0);

	auto blockPath = Environment::GetAbsPath("Resource/Assets/wall.jpg");
	m_BlockTexturePtr = new Texture(blockPath);

	m_StriderPtr = new Player(Demo::g_Id , Demo::g_Id, 120);
	m_StriderPtr->SetPos(4547, 1264);
	m_StriderPtr->SetBox();


	m_OtherPtr = new Player(-1, Demo::g_Id2, 120);
	m_OtherPtr->SetPos(990, 650);
	m_OtherPtr->SetBox();
	
	Line *l = new Line(Vec2(3,3),Vec2(5,5));
	l->Color() = Vec4(1,0.5,0,1);
	m_Render.AddObject(l);

	int birthPos[10][2] = 
	{
		{ 780, 700},
		{ 600, 900 },
		{ 1550, 300 },
		{ 600, 1900 },
		{ 400, 2000 },
		{ 2600, 400 },
		{ 3400, 800 },
		{ 4600, 900 },
		{ 2400, 300 },
		{ 600, 900 },
	};

	for (int i = 0; i < 10; i++)
	{
		Player* player = new Player(2+i ,1, 120);
		player->SetPos(birthPos[i][0], birthPos[i][1]);
       // player->SetBox();
		player->ResetDirAll(i % 8);
		m_NPCs.push_back(player);
		i++;
	}

	//TestServer();
}

Demo::~Demo()
{
	m_ClientPtr->Close();
}

void Demo::Update()
{
	double dt = Engine::GetInstance()->GetDeltaTime(); 
	
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
		if (m_ChangeStateTimeInterval >= 20 * dt )
		{
			m_IsChangeState = false;
			m_ChangeStateTimeInterval = 0;
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
			destBoxX = Random::NextInt(0, m_GameMapPtr->GetWidth() / 20 -1);
			destBoxY = Random::NextInt(0, m_GameMapPtr->GetHeight() / 20 -1);
			int velocity;
			velocity = Random::NextInt(150, 375);
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


	if (InputManager::GetInstance()->IsKeyUp(GLFW_KEY_KP_1)  || InputManager::GetInstance()->IsKeyUp(GLFW_KEY_1))
	{
		dir = static_cast<int>(FrameAnimation::Dir::S_W);
		m_StriderPtr->SetDir(dir);
	}

	if (InputManager::GetInstance()->IsKeyUp(GLFW_KEY_KP_7)  || InputManager::GetInstance()->IsKeyUp(GLFW_KEY_7))
	{

		dir = static_cast<int>(FrameAnimation::Dir::N_W);
		m_StriderPtr->SetDir(dir);
	}


	if (InputManager::GetInstance()->IsKeyUp(GLFW_KEY_KP_9)  || InputManager::GetInstance()->IsKeyUp(GLFW_KEY_9))
	{
		dir = static_cast<int>(FrameAnimation::Dir::N_E);
		m_StriderPtr->SetDir(dir);
	}


	if (InputManager::GetInstance()->IsKeyUp(GLFW_KEY_KP_2)  || InputManager::GetInstance()->IsKeyUp(GLFW_KEY_2))
	{
		dir = static_cast<int>(FrameAnimation::Dir::S);
		m_StriderPtr->SetDir(dir);
	}


	if (InputManager::GetInstance()->IsKeyUp(GLFW_KEY_KP_4)  || InputManager::GetInstance()->IsKeyUp(GLFW_KEY_4))
	{
		dir = static_cast<int>(FrameAnimation::Dir::W);
		m_StriderPtr->SetDir(dir);
	}

	if (InputManager::GetInstance()->IsKeyUp(GLFW_KEY_KP_8)  || InputManager::GetInstance()->IsKeyUp(GLFW_KEY_8))
	{

		dir = static_cast<int>(FrameAnimation::Dir::N);
		m_StriderPtr->SetDir(dir);
	}


	if (InputManager::GetInstance()->IsKeyUp(GLFW_KEY_KP_6)  || InputManager::GetInstance()->IsKeyUp(GLFW_KEY_6))
	{
		dir = static_cast<int>(FrameAnimation::Dir::E);
		m_StriderPtr->SetDir(dir);
	}

	if (InputManager::GetInstance()->IsKeyUp(GLFW_KEY_KP_5) && !m_IsChangeState)
	{
		m_IsChangeState = true;
		
		if ( m_StriderPtr->GetAnimationState() == Player::Moving)
		{
			m_StriderPtr->SetAnimationState(Player::Idle);
		}
		else
		{
			m_StriderPtr->SetAnimationState(Player::Moving);
		}
		m_StriderPtr->ResetDir(dir);
	}

}

ImVec4 clear_color = ImColor(114, 144, 154);
bool show_test_window = true;
bool show_another_window = false;
void Demo::Draw()
{
	
	m_GameMapPtr->Draw(m_RendererPtr, m_StriderPtr->GetX(), m_StriderPtr->GetY());

	
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
	
	m_GameMapPtr->DrawMask(m_RendererPtr, m_StriderPtr->GetX(), m_StriderPtr->GetY());
	
	//m_GameMapPtr->DrawCell(m_RendererPtr, mapOffsetX, mapOffsetY);
	m_StriderPtr->OnDraw(m_RendererPtr,px,py);

	m_OtherPtr->OnDraw(m_RendererPtr, m_OtherPtr->GetX() + mapOffsetX,m_OtherPtr->GetY() + mapOffsetY);

	/*for (Player* npc : m_NPCs)
	{
		npc->OnDraw(m_RendererPtr, npc->GetX() + mapOffsetX, npc->GetY() + mapOffsetY);
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
		 return ;
	 }
 }
