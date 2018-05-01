#pragma once
#include "defines.h"
#include "defineTypes.h"
#include "Singleton.h"

class BaseGameEntity;

enum GameMessageType
{
	MSG_GAME_STATR,
	MSG_COMBAT_ACTOR_ATTACK,
	MSG_COMBAT_ACTOR_DEFEND,
	MSG_PlayerCombatIdleState,
	MSG_PlayerCombatMoveState,
	MSG_PlayerCombatBackState,
	MSG_PlayerCombatAttackState,
	MSG_PlayerCombatBeAttackedState,
	MSG_PlayerCombatBeCastAttackedState,
	MSG_PlayerCombatGlobalState
};






struct Telegram
{
	int Sender;
	int Receiver;
	int MsgType;			//GameMessageType
	int64_t DispatchTime;
	void* ExtraInfo;
    
	bool operator < (const Telegram& telegram) const
    {
        return this->DispatchTime < telegram.DispatchTime;
    }
 
};

class MessageDispatcher final : public Singleton<MessageDispatcher>
{
private: 
	friend class Singleton<MessageDispatcher>;
	std::set<Telegram> m_MessageQueue;
	void Discharge(BaseGameEntity* pReceiver, const Telegram& msg);
	MessageDispatcher(){
		m_MessageQueue.clear();
	}
public:
	
	void DispatchMessage(int delay,
	int sender,
	int receiver,
	int msgType,
	void* ExtraInfo);

	void DispatchDelayedMessage();
};
#define MESSAGE_DISPATCHER_INSTANCE MessageDispatcher::GetInstance()

