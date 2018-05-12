#pragma once
#include "defines.h"
#include "defineTypes.h"
#include "Singleton.h"

class ActorManager : public Singleton<ActorManager>
{
public:
	ActorManager(){}
	~ActorManager(){}
	void Init(){};
	void Upate(){}
}

#define ACTOR_MANAGER_INSTANCE ActorManager::GetInstance()