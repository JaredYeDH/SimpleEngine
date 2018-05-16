#include "SplashScene.h"


SplashScene::SplashScene(int id,String name)
:BaseScene(id,name)
{
	m_State = Splash;
}
SplashScene::~SplashScene()
{

}


void SplashScene::Init() 
{

}
static bool s_bEnterGame =false;
void SplashScene::Update() 
{
	if(m_State == Splash)
	{
		if(ImGui::Button("Enter Game"))
		{
			m_State = Game;
		}
	}
	else 
	{
		
	}
	
}

void SplashScene::Draw() 
{

}
