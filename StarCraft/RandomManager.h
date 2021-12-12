#pragma once
#include "Singleton.h"

class RandomManager : public Singleton<RandomManager>
{
private:

public:
	void SetSeed(unsigned int seed);

	// min과 max를 포함한 값을 반환합니다.
	int GetValue(int min, int max);
};

