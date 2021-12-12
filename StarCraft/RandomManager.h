#pragma once
#include "Singleton.h"

class RandomManager : public Singleton<RandomManager>
{
private:

public:
	void SetSeed(unsigned int seed);

	// min�� max�� ������ ���� ��ȯ�մϴ�.
	int GetValue(int min, int max);
};

