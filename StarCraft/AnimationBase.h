#pragma once
#include "RenderConfig.h"

class AnimationBase
{
protected:
	int mCurRow = 0;
	int mCurCol = 0;
	Fix mElapsedTime = 0;
	Fix mOneFrameTime = 0;
	bool mbIsEnd = false;
public:
	AnimationBase() = default;
	virtual ~AnimationBase() = default;
	virtual void Play() = 0;
	virtual void Stop() = 0;
	virtual void Update() = 0;

	inline int GetRow() { return mCurRow; }
	inline int GetCol() { return mCurCol; }
	virtual void SetAngle(Fix degAngle) { }

	inline bool IsEnd() { return mbIsEnd; }
	virtual ID2D1Bitmap* GetBitmap() = 0;
};