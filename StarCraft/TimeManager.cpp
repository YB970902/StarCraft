#include "stdafx.h"
#include "TimeManager.h"

void TimeManager::Init(int fps)
{
	LONGLONG frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

	mFrequencyTime = 1.0 / frequency;

	LONGLONG count;
	QueryPerformanceCounter((LARGE_INTEGER*)&count);
	mCurCount = count;
	mPrevCount = count;

	mFPS = fps;
	mFrameTime = 1.0f / fps;
	mFixedDeltaTime = 1.0f / fps;
}

bool TimeManager::IsUpdateTime()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&mCurCount);

	mDeltaTime += (mCurCount - mPrevCount) * mFrequencyTime;
	mPrevCount = mCurCount;

	if (mDeltaTime >= mFrameTime)
	{
		mDeltaTime -= mFrameTime;
		return true;
	}
	return false;
}
