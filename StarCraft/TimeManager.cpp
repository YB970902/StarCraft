#include "stdafx.h"
#include "TimeManager.h"

void TimeManager::Init(int fps)
{
	LONGLONG frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

	m_frequencyTime = 1.0 / frequency;

	LONGLONG count;
	QueryPerformanceCounter((LARGE_INTEGER*)&count);
	m_currentCount = count;
	m_previousCount = count;

	m_fps = (float)fps;
	m_frameTime = 1.0 / fps;
}

bool TimeManager::IsUpdateTime()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_currentCount);

	m_deltaTime = (m_currentCount - m_previousCount) * m_frequencyTime;

	if (m_deltaTime > m_frameTime)
	{
		m_previousCount = m_currentCount;
		return true;
	}
	return false;
}
