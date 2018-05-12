#include "AlphaSence.h"
#include "core/Texture.h"
#include "../InputManager.h"
#include <iostream>

#include "../TextRenderer.h"
#include "../Bitmap.h"
#include "../combat/Combat.h"
#include "../animation/FrameAnimation.h"
#include "ThirdParty/NetEase/Sprite2.h"
#include "ThirdParty/NetEase/WDF.h"
#include "ThirdParty/NetEase/WAS.h"
// #include "../tsv.h"

// #include "animation_database.h"
Image* img;
TextRenderer* textRenderer;

struct Test1
{
	int a;
};
struct Test2
{
	Test1 dd;
	int b;
};

std::vector<FrameAnimation> g_SkillFrame(100);

std::vector<std::shared_ptr<Sprite2>> g_AllSprites;
static NetEase::WDF* g_Loader = nullptr;
std::vector<uint32> g_WASIDs; 
bool g_bChangeLock  = false;
AlphaSence::AlphaSence()
:m_Render()
{
	// SKILL_MANAGER_INSTANCE;
	// Test2 t2;
	// std::cout << "t2:" << std::hex << &t2
	// 	<< "\t t2.dd" << &t2.dd
	// 	<< "\t t2.b" << &t2.b
	// 	<< "\t t2.dd.a" << &t2.dd.a << std::endl;
	g_Loader =  new NetEase::WDF(Environment::GetWDFPath("magic.wdf"));
	// g_AllSprites = g_Loader->LoadAllSprite();
	g_WASIDs =  g_Loader->GetAllWASIDs();
	// g_Loader->SaveWAS(0x9EC0624E);
	// auto p = g_Loader->LoadSprite(0x2EB3476E);
	g_SkillFrame.clear();
	
 	g_SkillFrame.push_back (FrameAnimation(  g_Loader->LoadSprite(0x0E30CD44)));


	//TestTimer
	//TimerManager::GetInstance()->CreateTimer("Deda2", 1000, true, false, [this](){
	//	printf("void AlphaSence::Update(4000)\n");
	//	TimerManager::GetInstance()->CreateTimer("Deda3", 16, true, true,
	//		[](){DrawCircle(250, 150, 100, Vec4(0, 1, 0, 1), false);}	
	//	);
	//});
	// textRenderer = new TextRenderer();
	// utils::AnimDB db;
	// std::cout <<  db.query(3,10,3,-1)  << std::endl;
	

	// Line* l=new Line(Vec2(222,222),Vec2(111,111));
	// l->Color() = Vec4(1,0.5,0,1);
	
	// m_Render.AddObject(l);

	// Rect* r=new Rect(20,20,200,200);
	// r->Color() = Vec4(1,0.8,0,1);
	// m_Render.AddObject(r);

	// Circle* c=new Circle(100,100,88);
	// c->Color() = Vec4(1,0.5,0.2,1);
	// m_Render.AddObject(c);

	// img = new Image("/Users/oceancx/Documents/照片/湖工大-IMG_20130623_202050.jpg",Vec2(200,200),Vec2(400,300));
	// // c->Color() = Vec4(1,0.5,0.2,1);
	// m_Render.AddObject(img);


	// utils::AnimDB db;
	
	// for(int i=1;i<=20;i++)
	// {
	// 	int64 was_id = db.query(3,i,7,90);
	// 	// m_PlayerAnimation[i] = was_id;
	// 	// m_WeapAnimation[i] = wad_id;
	// 	if (was_id != -1){
	// 		std::cout << std::hex << was_id << std::endl;
	// 		db.printInfo(db.getRowWas( was_id));
	// 	}
       		
	// }

	// Bitmap bitmap;
	//bitmap.CreateBitmapFile(201 ,201, 8, "D:\\a.bmp");
	// BitmapFile f;
	//bitmap.Load("F:\\svn\\misc\\2d_logicmap\\2\\areas_0_-43916_18934_85194_47922",f);
	//bitmap.Save("D:\\a.bmp", f);

}

AlphaSence::~AlphaSence()
{
	
}

static int g_CurrentSprite = -1;
void AlphaSence::Update() 
{
	//if(g_bChangeLock) return;
	double dt = ENGINE_INSTANCE->GetDeltaTime();
	for(auto& f : g_SkillFrame)
		f.OnUpdate(dt);
	
    // lua_settop(L,0);
    // lua_getglobal(L,"OnUpdate");
    // lua_pcall(L,0,0,0);
    INPUT_MANAGER_INSTANCE->RegisterOnKeyClickEvent(GLFW_KEY_1,[]()
	{
		if(g_CurrentSprite >= g_WASIDs.size())
			g_CurrentSprite = 0;
		
//        g_SkillFrame.clear();
		g_bChangeLock = true;
		int cnt = 0;
		while(true && g_CurrentSprite < g_WASIDs.size())
		{

			auto res = g_Loader->LoadSprite(g_WASIDs[g_CurrentSprite++]);
			if(!res->Error)
			{
				g_SkillFrame[cnt] = FrameAnimation( res );
				int x = cnt % 12 ;
				int y = cnt / 12 ;
				g_SkillFrame[cnt].SetPos({x*120,y*120});
				cnt++;
				if(cnt == 80)
				{
					g_bChangeLock = false;
					return;
				}
					
			}
			
		}
        g_bChangeLock = false;
        return;

		
	});

	
}



void AlphaSence::Draw() 
{
	if(g_bChangeLock) return;
	for(auto& f : g_SkillFrame)
		f.Draw();
	//textRenderer->Draw();
	//m_Render.Render();
}

