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
private:
	
	std::map<int,Scene*> m_Scenes;
	Scene* m_pCurrentScene;
	utils::tsv m_MapTSV;
};

#define SCENE_MANAGER_INSTANCE SceneManager::GetInstance()
