#pragma once

#include "defines.h"
#include "Singleton.h"
#include "IUpdateDraw.h"
#include "Network/Client.h"
#include "Network/Message.h"
#include "Time/TimerManager.h"
/*
* 引擎主要负责各种Manger的载入和初始化
比方说： SenceManager TextureManger  
*/
class Engine : public Singleton<Engine>
{
public:
	friend Singleton<Engine>;

	~Engine();
	void Init();
	void Update(double dt);
	void Draw();
	void Destroy();
	void SetClient(Client* client);
	double GetDeltaTime() { return m_DeltaTime; }
	double GetDeltaTimeMilliseconds() { return m_DeltaTime * 1000; }
	void DispatchMove(MoveMessage msg);
private:
	Engine();
	double m_DeltaTime;
	IUpdateDraw * mSence;
	

};