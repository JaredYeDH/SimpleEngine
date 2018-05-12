#pragma once
#include "defines.h"
#include "defineTypes.h"
#include "Singleton.h"
#include "IUpdateDraw.h"
class SceneManager : public Singleton<SceneManager>
{
public:
	SceneManager(){}
	~SceneManager(){}
	void Init();
	void Update();
	void Draw();
private:
	IUpdateDraw * mSence;
};

#define SCENE_MANAGER_INSTANCE SceneManager::GetInstance()
