#pragma once

class IMouseEvent
{
public:
	virtual void OnEvent(int button, int action, int mods) = 0;
};


class Event
{
public:
	Event();
	~Event();
	
};