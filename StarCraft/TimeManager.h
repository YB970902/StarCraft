#pragma once
#include "Singleton.h"

class TimeManager : public Singleton<TimeManager>
{
protected:
	LONGLONG m_currentCount;
	LONGLONG m_previousCount;

	float m_frequencyTime;
	float m_frameTime;
	float m_deltaTime;

	int m_fps;
public:
	void Init(int fps);

	bool IsUpdateTime();

	inline int GetFPS() { return m_fps; }
	inline float GetDeltaTime() { return m_deltaTime; }
};