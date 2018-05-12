#pragma once
#include "defines.h"
#include "defineTypes.h"
#include "simple_engine.h"
#include "../IUpdateDraw.h"
class BaseScene
{
public:
	BaseScene(int id,String name);
	~BaseScene();
	void Init();
	void Update();
	void Draw();
private:
	int m_ID;
	String m_Name;
	
	IUpdateDraw * mSence;
};
