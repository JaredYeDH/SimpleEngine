#include "Random.h"
#include <cstdlib>
#include <ctime>

int Random::NextInt(int rangeMin, int rangeMax)
{
	std::srand(std::time(0)); 
	int randomVariable = std::rand();
	return randomVariable % (rangeMax - rangeMin+1) + rangeMin;
}

