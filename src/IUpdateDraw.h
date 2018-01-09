#pragma once

class IUpdateDraw
{
public:
	virtual void Update() = 0;
	virtual void Draw() = 0;
};