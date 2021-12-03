#include "stdafx.h"
#include "MultipleAnimation.h"

MultipleAnimation::MultipleAnimation(const MultipleAnimationClip& clip)
	: mClip{ clip }
{
	mOneFrameTime = mClip.AnimationTime / (Fix)mClip.Length;
	mElapsedTime = 0;
	mbIsEnd = false;
}

MultipleAnimation::~MultipleAnimation()
{
}

void MultipleAnimation::Play()
{
	mbIsEnd = false;
	mCurRow = mClip.StartRow;
}

void MultipleAnimation::Init(ID2D1Bitmap* pLeftBitmap, ID2D1Bitmap* pRightBitmap)
{
	mpLeftBitmap = pLeftBitmap;
	mpCurBitmap = pRightBitmap;
	mpRightBitmap = pRightBitmap;
}

void MultipleAnimation::Stop()
{
	mbIsEnd = true;
	mCurRow = mClip.StartRow;
}

void MultipleAnimation::Update()
{
	if (mbIsEnd) { return; }
	mElapsedTime += DELTA_TIME;
	if (mElapsedTime >= mOneFrameTime)
	{
		mElapsedTime -= mOneFrameTime;
		mCurRow += (int)mClip.Direction;
		if (mCurRow == mClip.StartRow + mClip.Length * (int)mClip.Direction)
		{
			if (mClip.IsLoop) { mCurRow = mClip.StartRow; }
			else
			{
				mCurRow -= (int)mClip.Direction;
				mbIsEnd = true;
			}
		}
	}
}

void MultipleAnimation::SetAngle(Fix degAngle)
{
	int col = (degAngle + (Fix)90) / ONE_DIR_ANGLE;
	col += HALF_DIR_COUNT;
	if (col >= DIR_COUNT) { col -= DIR_COUNT; }

	if (col != mLookDir)
	{
		mLookDir = col;
		if (col < HALF_DIR_COUNT)
		{
			mpCurBitmap = mpLeftBitmap;
			mCurCol = col;
		}
		else
		{
			mpCurBitmap = mpRightBitmap;
			mCurCol = HALF_DIR_COUNT - (col - HALF_DIR_COUNT);
		}
	}
}
