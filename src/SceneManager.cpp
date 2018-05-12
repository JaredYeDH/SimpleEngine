#include "SceneManager.h"
#include "Projects/Demo.h"

void SceneManager::Init() 
{
	//mSence = new TestNetwork();
	//mSence = new AlphaSence();
	mSence = new Demo();
};

void SceneManager::Update() 
{
	mSence->Update();
};

void SceneManager::Draw() 
{
	mSence->Draw();
};
