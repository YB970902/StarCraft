#pragma once
#include "Singleton.h"

class TimeManager : public Singleton<TimeManager>
{
protected:
	LONGLONG mCurCount = 0;
	LONGLONG mPrevCount = 0;

	float mFrequencyTime = 0.0f;
	float mFrameTime = 0.0f;
	float mDeltaTime = 0.0f;

	int mFPS = 0;

	Fix mFixedDeltaTime = 0;
public:
	void Init(int fps);

	bool IsUpdateTime();

	inline int GetFPS() { return mFPS; }
	inline Fix GetDeltaTime() { return mFixedDeltaTime; }
	inline float RealDeltaTime() { return mDeltaTime; }
};