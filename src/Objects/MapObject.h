#pragma once
#include "Object.h"

/*
用来装地图的Object 
最简单的做法，把地图读出来，然后生成一张texture载入到GPU就行了
*/
class MapObject : public Object
{
public:
	MapObject();
	MapObject(const std::string & name);
	~MapObject();

	void Reset();
	void Initialize();
	void AfterInitialized();
	void Update();
	void Draw();
	
};