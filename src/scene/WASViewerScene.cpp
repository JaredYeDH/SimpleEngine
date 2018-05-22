#include "WASViewerScene.h"
#include "SceneManager.h"
#include <scene/SplashScene.h>

static std::vector<uint32> s_WASIDs;
static std::map<uint32,String> s_WASIDStrings;
static NE::WDF* s_pWDF;
static FrameAnimation* s_FrameAnimation;
static std::map<uint32,FrameAnimation*> s_LoadFrames;
static int s_LastWASIndex = 0;
static int s_CurrentWASIndex = 0;
static bool s_ShowListBox = true;

void SwitchFrameAnimation()
{	
	s_FrameAnimation = nullptr;
	s_LastWASIndex = s_CurrentWASIndex;
	if(s_CurrentWASIndex >= s_WASIDs.size()) return;
	auto id = s_WASIDs[s_CurrentWASIndex];
	if(!s_pWDF) return;

	Sprite* sprite=nullptr;
	if(s_LoadFrames.count(id) <=0)
	{
		sprite = s_pWDF->LoadSprite(id);
		if(!sprite ){return;} 
		s_LoadFrames[id] = new FrameAnimation(sprite);
	}
	s_FrameAnimation = s_LoadFrames[id];
	s_FrameAnimation->SetPivotPos({SCREEN_WIDTH/2 ,SCREEN_HEIGHT/2});
	s_LastWASIndex = s_CurrentWASIndex;
}

WASViewerScene::WASViewerScene(int id,String name)
:BaseScene(id,name)
{
	m_State = Splash;
	s_FrameAnimation =nullptr;
}

WASViewerScene::~WASViewerScene()
{

}

void WASViewerScene::Reset(String wdf)
{
	if (s_pWDF)
	{
		delete s_pWDF;
		s_pWDF = nullptr;
	}
	s_FrameAnimation = nullptr;
	s_pWDF = new NE::WDF(Environment::GetWDFPath(wdf));
	s_WASIDs.clear();
	s_WASIDs = s_pWDF->GetAllWASIDs();

	for(auto& it : s_LoadFrames)
	{
		delete it.second;
		it.second = nullptr;
	}
	s_LoadFrames.clear();
	TEXTURE_MANAGER_INSTANCE->ClearAll();

	s_WASIDStrings.clear();

	utils::tsv wasTSV(Environment::GetTablePath(String(wdf+".ini")));
	for(int i=0;i< wasTSV.tabRows.size() ; i++)
	{
		auto row = wasTSV.tabRows[i];
		
		uint32 id = std::stoul(row[0],0,16);
		auto name = row[1];
		std::cout << name << std::endl;
		s_WASIDStrings[id]=name;
	}

	for(auto& id : s_WASIDs)
	{
		if(s_WASIDStrings.count(id) > 0)continue;
		s_WASIDStrings[id]= utils::Int2Hex(id);
	}
	std::sort(s_WASIDs.begin(),s_WASIDs.end(),[](uint32 lhs,uint32 rhs){
		return s_WASIDStrings[lhs] > s_WASIDStrings[rhs];
	});

	s_LastWASIndex = s_CurrentWASIndex = -1;
	SwitchFrameAnimation();
	
}

void WASViewerScene::Init() 
{
	
}

void WASViewerScene::Update() 
{
	if(s_ShowListBox)
	{
		//static int listbox_item_current2 = 2;
            //ImGui::PushItemWidth(-1);
            //ImGui::ListBox("##listbox2", &listbox_item_current2, listbox_items, IM_ARRAYSIZE(listbox_items), 4);
            //ImGui::PopItemWidth();
		if(ImGui::Button("Back"))
		{
			SCENE_MANAGER_INSTANCE->SwitchScene("Splash");
			SplashScene* p =(SplashScene*) SCENE_MANAGER_INSTANCE->GetScene("Splash");
			p->Reset();
			return;
		}
		if(s_WASIDs.size()>0)
		{
			ImGui::PushItemWidth(-1);	
			ImGui::ListBox("##List", &s_CurrentWASIndex, [](void* data, int idx, const char** out_text)
			{
				std::vector<uint32> * p = (std::vector<uint32> *)data;
				uint32 id = (*p)[idx];
				*out_text = s_WASIDStrings[id].c_str();
				return true;
			}, (void*)&s_WASIDs, s_WASIDs.size(), 20);
			
			if(s_LastWASIndex != s_CurrentWASIndex)
			{
				SwitchFrameAnimation();
				
			}
			ImGui::PopItemWidth();	
		}			
	}
	if(s_FrameAnimation)
		s_FrameAnimation->OnUpdate();
	
}

void WASViewerScene::Draw() 
{
	if(s_FrameAnimation)
		s_FrameAnimation->Draw();
}
