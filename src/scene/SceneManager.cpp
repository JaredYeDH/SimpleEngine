#include "SceneManager.h"
#include "Demo.h"

SceneManager::SceneManager()
:m_pCurrentScene(nullptr),
m_MapTSV(utils::tsv(Environment::GetTsvPath("map")))
{
	m_Scenes.clear();

	for(auto& row : m_MapTSV.tabRows)
	{
		int id = std::stoi(row[0]);
		String name = row[1];
		m_Scenes[id] = new BaseScene(id,name);
	}

	if(m_Scenes.size() > 0)
	{
		m_pCurrentScene = m_Scenes.begin()->second;
		m_pCurrentScene->Init();
	}
	
}

SceneManager::~SceneManager()
{

}

void SceneManager::Init() 
{
	
};

void SceneManager::Update() 
{
	if(m_pCurrentScene)
	{
		m_pCurrentScene->Update();
	}
};

void SceneManager::Draw() 
{
	if(m_pCurrentScene)
	{
		m_pCurrentScene->Draw();
	}
};
