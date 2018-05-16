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
	virtual void Init();
	virtual void Update();
	virtual void Draw();
	String GetName(){return m_Name;};
	int GetID(){return m_ID;};
protected:
	int m_ID;
	String m_Name;

};
