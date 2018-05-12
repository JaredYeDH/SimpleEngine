#pragma once

#include "defines.h"
#include "Singleton.h"
#include "IUpdateDraw.h"
#include "Network/Client.h"
#include "Network/Message.h"
#include "Time/TimerManager.h"

class Engine : public Singleton<Engine>
{
public:
	friend Singleton<Engine>;

	~Engine();
	void Init();
	void Update();
	void Draw();
	void Destroy();
	double GetDeltaTime() { return m_DeltaTime; }
	double GetDeltaTimeMilliseconds() { return m_DeltaTime * 1000; }
	
private:
	Engine();
	double m_DeltaTime;
	double m_LastTime;
};

#define ENGINE_INSTANCE Engine::GetInstance()