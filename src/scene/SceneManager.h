#pragma once
#include "defines.h"
#include "defineTypes.h"
#include "../simple_engine.h"
#include "Scene.h"

class SceneManager : public Singleton<SceneManager>
{
public:
	SceneManager();
	~SceneManager();
	void Init();
	void Update();
	void Draw();
	void SwitchScene(String name);
	void AddScene(BaseScene* scene) 
	{ 
		if(!scene) return ;
		
		if( m_Scenes.count( scene->GetName() ) > 0 ) return ;
 		m_Scenes[scene->GetName()]=scene;
	};
	BaseScene* GetScene(String name){return m_Scenes.count(name)>0?m_Scenes[name]:nullptr; }
	void RemoveScene(String name) { m_Scenes.erase(name);   };
private:
	
	std::map<String,BaseScene*> m_Scenes;
	BaseScene* m_pCurrentScene;
	BaseScene* m_pNextScene;
	utils::tsv m_MapTSV;
};

#define SCENE_MANAGER_INSTANCE SceneManager::GetInstance()
