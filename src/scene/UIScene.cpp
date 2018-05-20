#include "UIScene.h"
#include "SceneManager.h"
#include "WASViewerScene.h"
#include "../core/Graphics2D.h"
#include "../core/Renderer.h"
#include "ResourceManager.h"



struct Button
{
	Button(FrameAnimation* frame)
	{
		m_Frame = frame;
		lastx = lasty = 0;
		m_bDragStart = false;
		INPUT_MANAGER_INSTANCE->RegisterOnKeyClickEvent(GLFW_MOUSE_BUTTON_LEFT, [this]() {
			if (CheckEvent())
			{
				OnClick();
			}
		});

		INPUT_MANAGER_INSTANCE->RegisterOnKeyDownEvent(GLFW_MOUSE_BUTTON_LEFT, [this]() {
			//if (CheckEvent())
			//{
			//	/*if (!m_bDragStart)
			//	{
			//		x= INPUT_MANAGER_INSTANCE->GetMouseX();
			//		y= INPUT_MANAGER_INSTANCE->GetMouseY();
			//	}*/
			//	
			//	m_bDragStart = true;
			//	OnStartDrag();
			//}
			//else
			//{
			//	if (m_bDragStart)
			//	{
			//		m_bDragStart = false;
			//		OnEndDrag();
			//	}
			//}
			
			if (!m_bDragStart && CheckEvent())
			{
				m_bDragStart = true;
				lastx = INPUT_MANAGER_INSTANCE->GetMouseX();
				lasty = INPUT_MANAGER_INSTANCE->GetMouseY();
			}
			if(m_bDragStart)
			OnStartDrag();
			
		});

		INPUT_MANAGER_INSTANCE->RegisterOnKeyUpEvent(GLFW_MOUSE_BUTTON_LEFT, [this]() {
			/*if (m_bDragStart)
			{
				m_bDragStart = false;
				OnEndDrag();
			}*/
			if (m_bDragStart)
			{
				OnEndDrag();
				m_bDragStart = false;
			}
			
		});
	}

	~Button()
	{

	}

	void OnClick()
	{
		std::cout << " m clicked!" << std::endl;
		std::cout << "click x:" << INPUT_MANAGER_INSTANCE->GetMouseX() << " click y:" << INPUT_MANAGER_INSTANCE->GetMouseY() << std::endl;
	}

	void OnStartDrag()
	{
		int mx = INPUT_MANAGER_INSTANCE->GetMouseX();
		int my = INPUT_MANAGER_INSTANCE->GetMouseY();
		std::cout << "void OnStartDrag()\t x:" <<  mx << "\t y:" << my <<  std::endl;
		//m_Frame->SetPos({ m_Frame->GetPos().x + ( mx - x), m_Frame->GetPos().y+( my -y) });
	//	x = mx;
		//y = my;
		
		int px = m_Frame->GetPos().x + (mx - lastx);
		int py = m_Frame->GetPos().y + (my - lasty);
		m_Frame->SetPos({ px,py });
		lastx = mx;
		lasty = my;

	}
	void OnEndDrag()
	{
		std::cout << "void OnEndDrag()" << std::endl;
	}

	bool CheckEvent()
	{
		if (!m_Frame) return false;

		auto bound = m_Frame->GetBound();
		int mx = INPUT_MANAGER_INSTANCE->GetMouseX();
		int my = INPUT_MANAGER_INSTANCE->GetMouseY();
		return utils::BoundHitTest(bound, { mx,my });
	}

	FrameAnimation* m_Frame;
	bool  m_bDragStart ;
	int lastx, lasty;
};


UIScene::UIScene(int id, String name)
	:BaseScene(id, name)
{

}

UIScene::~UIScene()
{

}

Button* pButton;
FrameAnimation* test;



void UIScene::Init()
{
	RENDERER_2D_INSTANCE->AddObject(new Line({ 0,0 }, {50,50}));
	//0x1732c1ef 0x49386FCE
	FrameAnimation* f = new FrameAnimation(RESOURCE_MANAGER_INSTANCE->LoadWASSprite( ResourceManager::SHAPEWDF  ,0x49386FCE));
	 test = new FrameAnimation(RESOURCE_MANAGER_INSTANCE->LoadWASSprite( ResourceManager::WZIFEWDF ,0x1732c1ef ));
	 test->SetPos({180,130});
	pButton = new Button(f);

	TextRenderer::GetInstance();
}


void UIScene::Update()
{
	
	pButton->m_Frame->OnUpdate();
	test->OnUpdate();
	
}

void UIScene::Draw()
{
	pButton->m_Frame->Draw();
	test->Draw();
	
	RENDERER_2D_INSTANCE->Render();


	
}
