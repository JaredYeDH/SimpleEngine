#pragma once
#include "Singleton.h"
class Random : public Singleton<Random>
{
public:
	Random() ;
	~Random() {};
	int NextInt(int rangeMin,int rangeMax);
};
#define RANDOM_INSTANCE Random::GetInstance()