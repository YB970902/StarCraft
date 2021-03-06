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
	mFixedDeltaTime = (Fix)1.0f / (Fix)fps;
}

bool TimeManager::IsUpdateTime()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&mCurCount);

	mDeltaTime += (mCurCount - mPrevCount) * mFrequencyTime;
	mPrevCount = mCurCount;

	if (mDeltaTime >= mFrameTime)
	{
		cout << mDeltaTime << endl;
		mDeltaTime = 0;
		return true;
	}
	return false;
}
