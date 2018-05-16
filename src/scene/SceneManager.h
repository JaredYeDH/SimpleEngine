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
	void AddScene(String name ,BaseScene* scene) {   m_Scenes[name] = scene; };
	void RemoveScene(String name) { m_Scenes.erase(name);   };
private:
	
	std::map<String,BaseScene*> m_Scenes;
	BaseScene* m_pCurrentScene;
	BaseScene* m_pNextScene;
	utils::tsv m_MapTSV;
};

#define SCENE_MANAGER_INSTANCE SceneManager::GetInstance()
