#include "stdafx.h"
#include "RandomManager.h"

void RandomManager::SetSeed(unsigned int seed)
{
	srand(seed);
}

int RandomManager::GetValue(int min, int max)
{
	return rand() % (max - min + 1) + min;
}
