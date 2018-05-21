#include "SplashScene.h"
#include "SceneManager.h"
#include "WASViewerScene.h"
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

static int current_wdf_idx=0;
static int last_wdf_idx=0;
void SplashScene::Reset()
{
	current_wdf_idx=last_wdf_idx=0;
}
void SplashScene::Update() 
{
	if(m_State == Splash)
	{
		static const char* list_items[] = {"addon.wdf","atom.wdf","chat.wdf","firework.wdf","goods.wdf","item.wdf","magic.wdf","mapani.wdf","mhimage.wdf","misc.wdf","music.wdf","scene.wdf","shape.wd1","shape.wd2","shape.wd3","shape.wd4","shape.wd5","shape.wd6","shape.wd7","shape.wdf","smap.wdf","sound.wdf","stock.wdf","waddon.wdf","wzife.wd1","wzife.wdf","wzimage.wdf"};
		int list_size = sizeof(list_items)/sizeof(*list_items);
		ImGui::ListBox("##wdf_list",&current_wdf_idx,list_items,list_size, list_size);
		if(current_wdf_idx < list_size && last_wdf_idx!=current_wdf_idx)
		{
			
			WASViewerScene* p =(WASViewerScene*) SCENE_MANAGER_INSTANCE->GetScene("WASViewer");
			p->Reset(list_items[current_wdf_idx]);
			SCENE_MANAGER_INSTANCE->SwitchScene("WASViewer");
		}
	}
	else 
	{

	}
	
}

void SplashScene::Draw() 
{

}
