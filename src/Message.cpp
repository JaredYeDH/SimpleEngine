#include "Message.h"
#include <chrono>
#include "Entity.h"

void MessageDispatcher::Discharge(BaseGameEntity* pReceiver, const Telegram& msg)
{
    if(pReceiver)
	pReceiver->HandleMessage(msg);
}


void MessageDispatcher::DispatchMessage(int delay,
	int sender,
	int receiver,
	int msg,
	void* extraInfo)
{
	BaseGameEntity* pReceiver = GAME_ENTITY_MANAGER_INSTANCE->GetEntityByID(receiver);

    Telegram telegram;
    telegram.Sender = sender;
    telegram.Receiver = receiver;
    telegram.Msg = msg;
    telegram.ExtraInfo = extraInfo;
	if(delay <= 0 )
	{
		Discharge(pReceiver,telegram);
	}
	else
	{
    	auto now = std::chrono::system_clock::now();
   		auto currentTime = now.time_since_epoch().count() /1000;
		telegram.DispatchTime = currentTime + delay;
		m_MessageQueue.insert(telegram);
	}
}

void MessageDispatcher::DispatchDelayedMessage()
{
   	auto now = std::chrono::system_clock::now();
   	auto currentTime = now.time_since_epoch().count() /1000;

	while(m_MessageQueue.size()>0&& (m_MessageQueue.begin()->DispatchTime < currentTime) &&
		(m_MessageQueue.begin()->DispatchTime > 0) )
	{
		Telegram telegram = *m_MessageQueue.begin();
		BaseGameEntity* pReceiver = GAME_ENTITY_MANAGER_INSTANCE->GetEntityByID(telegram.Receiver);
		Discharge(pReceiver,telegram);
		m_MessageQueue.erase(m_MessageQueue.begin());
	}
}
