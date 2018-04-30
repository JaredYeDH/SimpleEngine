#pragma once
#include "defines.h"
#include "defineTypes.h"
#include "Singleton.h"

class BaseGameEntity;

enum GameMessageType
{
	MSG_GAME_STATR,
	MSG_COMBAT_ACTOR_ATTACK,
	MSG_COMBAT_ACTOR_DEFEND
};


struct Telegram
{
	int Sender;
	int Receiver;
	int Msg;			//GameMessageType
	time_t DispatchTime;
	void* ExtraInfo;
    
	bool operator < (const Telegram& telegram) const
    {
        return this->DispatchTime < telegram.DispatchTime;
    }
 
};

class MessageDispatcher final : Singleton<MessageDispatcher>
{
private: 
	std::set<Telegram> m_MessageQueue;
	void Discharge(BaseGameEntity* pReceiver, const Telegram& msg);
	MessageDispatcher(){}
public:
	
	void DispatchMessage(double delay,
	int sender,
	int receiver,
	int msg,
	void* ExtraInfo);

	void DispatchDelayedMessage();
};
#define MESSAGE_DISPATCHER_INSTANCE MessageDispatcher::GetInstance();

