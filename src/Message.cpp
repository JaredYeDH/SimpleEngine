#include "Message.h"
#include <chrono>
#include "Entity.h"

void MessageDispatcher::Discharge(BaseGameEntity* pReceiver, const Telegram& msg)
{
	pReceiver->HandleMessage(msg);
}


void MessageDispatcher::DispatchMessage(double delay,
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
	if(delay <= 0.0)
	{
		Discharge(pReceiver,telegram);
	}
	else
	{
        std::chrono::system_clock::time_point now =std::chrono::system_clock::now();
		time_t currentTime =std::chrono::system_clock::to_time_t(now );
		telegram.DispatchTime = currentTime + delay;
		m_MessageQueue.insert(telegram);
	}
}

void MessageDispatcher::DispatchDelayedMessage()
{
    std::chrono::system_clock::time_point now =std::chrono::system_clock::now();
    time_t currentTime =std::chrono::system_clock::to_time_t(now );

	while( (m_MessageQueue.begin()->DispatchTime < currentTime) &&
		(m_MessageQueue.begin()->DispatchTime > 0) )
	{
		Telegram telegram = *m_MessageQueue.begin();
		BaseGameEntity* pReceiver = GAME_ENTITY_MANAGER_INSTANCE->GetEntityByID(telegram.Receiver);
		Discharge(pReceiver,telegram);
		m_MessageQueue.erase(m_MessageQueue.begin());
	}
}
