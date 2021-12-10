#pragma once
#include "AnimationBase.h"
#include "AnimationData.h"

const Fix ONE_DIR_ANGLE = 11.25f;
const int DIR_COUNT = 32;
const int HALF_DIR_COUNT = 16;
const int HALF_HALF_DIR_COUNT = 8;

class MultipleAnimation : public AnimationBase
{
private:
	ID2D1Bitmap* mpLeftBitmap = nullptr;
	ID2D1Bitmap* mpRightBitmap = nullptr;
	ID2D1Bitmap* mpCurBitmap = nullptr;

	MultipleAnimationClip mClip;

	int mLookDir = 0;
public:
	MultipleAnimation(const MultipleAnimationClip& clip);
	virtual ~MultipleAnimation();

	void Init(ID2D1Bitmap* pLeftBitmap, ID2D1Bitmap* pRightBitmap);

	virtual void Play() override;
	virtual void Stop() override;
	virtual void Update() override;

	virtual void SetAngle(Fix degAngle) override;
	inline bool IsEnd() { return mbIsEnd; }
	inline ID2D1Bitmap* GetBitmap() { return mpCurBitmap; }
};