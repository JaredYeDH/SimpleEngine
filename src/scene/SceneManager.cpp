#include "SceneManager.h"
#include "../Random.h"
SceneManager::SceneManager()
:m_pCurrentScene(nullptr),
m_MapTSV(utils::tsv(Environment::GetTsvPath("map")))
{
	m_Scenes.clear();

	for(auto& row : m_MapTSV.tabRows)
	{
		int id = std::stoi(row[0]);
        String name = row.size()>1?row[1]:"";
		m_Scenes[name] = new Scene(id,name);
	}

	if(m_Scenes.size() > 0)
	{
		int randomMapID = RANDOM_INSTANCE->NextInt(0,m_Scenes.size()-1);
		auto iter = m_Scenes.begin();
		for(;randomMapID>=0;iter++,randomMapID--);
		m_pCurrentScene = iter->second;
	//	m_pCurrentScene->Init();
	}
	m_pNextScene = nullptr;
	
}

SceneManager::~SceneManager()
{

}

void SceneManager::Init() 
{
	
};

void SceneManager::SwitchScene(String name)
{
	auto it = m_Scenes.find(name);
	if(it!=m_Scenes.end())
	{
		m_pNextScene = it->second;
	}
}

void SceneManager::Update() 
{
	if(m_pNextScene!=nullptr)
	{
		m_pCurrentScene = m_pNextScene;
		m_pNextScene = nullptr;
	}
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
