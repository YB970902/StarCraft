#pragma once
#include "AnimationBase.h"
#include "AnimationData.h"

class SingleAnimation : public AnimationBase
{
private:
	ID2D1Bitmap* mpCurBitmap = nullptr;
	SingleAnimationClip mClip;

	int mCurRepeatCount = 0;
public:
	SingleAnimation(const SingleAnimationClip& clip);
	virtual ~SingleAnimation();

	void Init(ID2D1Bitmap* pBitmap);

	virtual void Play() override;
	virtual void Stop() override;
	virtual void Update() override;

	inline bool IsEnd() { return mbIsEnd; }
	inline ID2D1Bitmap* GetBitmap() { return mpCurBitmap; }
};