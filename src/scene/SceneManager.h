#pragma once
#include "defines.h"
#include "defineTypes.h"
#include "../simple_engine.h"
#include "BaseScene.h"

class SceneManager : public Singleton<SceneManager>
{
public:
	SceneManager();
	~SceneManager();
	void Init();
	void Update();
	void Draw();
private:
	
	std::map<int,BaseScene*> m_Scenes;
	BaseScene* m_pCurrentScene;
	utils::tsv m_MapTSV;
};

#define SCENE_MANAGER_INSTANCE SceneManager::GetInstance()
