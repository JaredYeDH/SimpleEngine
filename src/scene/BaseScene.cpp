#include "BaseScene.h"
#include "Demo.h"

BaseScene::BaseScene(int id,String name)
:m_ID(id),m_Name(name)
{
	
}
BaseScene::~BaseScene()
{

}
void BaseScene::Init()
{
	//mSence = new TestNetwork();
	//mSence = new AlphaSence();
	mSence = new Demo();
}
void BaseScene::Update()
{
	mSence->Update();
}
void BaseScene::Draw()
{
	mSence->Draw();
}
