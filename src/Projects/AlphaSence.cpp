#include "AlphaSence.h"
#include "core/Texture.h"
#include "../InputManager.h"
#include <iostream>


Image* img;


AlphaSence::AlphaSence()
:m_Render()
{
	//TestTimer
	//TimerManager::GetInstance()->CreateTimer("Deda2", 1000, true, false, [this](){
	//	printf("void AlphaSence::Update(4000)\n");
	//	TimerManager::GetInstance()->CreateTimer("Deda3", 16, true, true,
	//		[](){DrawCircle(250, 150, 100, Vec4(0, 1, 0, 1), false);}	
	//	);
	//});
    using string = std::string;
    printf("");
    
	Line* l=new Line(Vec2(222,222),Vec2(111,111));
	l->Color() = Vec4(1,0.5,0,1);
	
	m_Render.AddObject(l);

	Rect* r=new Rect(20,20,200,200);
	r->Color() = Vec4(1,0.8,0,1);
	m_Render.AddObject(r);

	Circle* c=new Circle(100,100,88);
	c->Color() = Vec4(1,0.5,0.2,1);
	m_Render.AddObject(c);

	img = new Image("/Users/oceancx/Documents/照片/湖工大-IMG_20130623_202050.jpg",Vec2(200,200),Vec2(400,300));
	// c->Color() = Vec4(1,0.5,0.2,1);
	m_Render.AddObject(img);

}

AlphaSence::~AlphaSence()
{
	
}

bool toggle = false;
bool s = false;
void AlphaSence::Update() 
{
    // lua_settop(L,0);
    // lua_getglobal(L,"OnUpdate");
    // lua_pcall(L,0,0,0);

	if (InputManager::GetInstance()->IsKeyDown(GLFW_KEY_LEFT_SUPER)  )
	{	
		
			
		if(InputManager::GetInstance()->IsKeyDown(GLFW_KEY_E))
		{
			if(s ) return ;
			s = true;
			toggle = !toggle;				
			// toggle = !toggle;
		
		// img->T().pos.x++;
		// img->T().pos.y += 2;
		// if (img->T().pos.y > 500)
		// {
		// 	img->T().pos.x=0;
		// 	img->T().pos.y = 0;
		// }	
		}

		if(InputManager::GetInstance()->IsKeyUp(GLFW_KEY_E))
		{
			img->SetVisible(toggle);
			s=false;
		}
	}

	
}



void AlphaSence::Draw() 
{
	m_Render.Render();
}

