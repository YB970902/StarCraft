#include "stdafx.h"
#include "SingleAnimation.h"

SingleAnimation::SingleAnimation(const SingleAnimationClip& clip)
	:mClip{ clip }
{
	mOneFrameTime = mClip.AnimationTime / (Fix)mClip.Length;
	mElapsedTime = 0;
	mbIsEnd = false;
}

SingleAnimation::~SingleAnimation()
{
}

void SingleAnimation::Init(ID2D1Bitmap* pBitmap)
{
	mpCurBitmap = pBitmap;
}

void SingleAnimation::Play()
{
	mbIsEnd = false;
	mCurRow = mClip.StartFrameY;
	mCurCol = mClip.StartFrameX;
}

void SingleAnimation::Stop()
{
	mbIsEnd = true;
	mCurRow = mClip.StartFrameY;
	mCurCol = mClip.StartFrameX;
}

void SingleAnimation::Update()
{
	if (mbIsEnd) { return; }
	mElapsedTime += DELTA_TIME;
	if (mElapsedTime >= mOneFrameTime)
	{
		mElapsedTime -= mOneFrameTime;
		switch (mClip.Direction)
		{
		case eSingleClipDirection::Right:
			mCurCol += (int)mClip.Direction;
			if (mCurCol == mClip.StartFrameX + mClip.Length)
			{
				if (mClip.IsLoop) { mCurCol = mClip.StartFrameX; }
				else
				{
					--mCurCol;
					mbIsEnd = true;
				}
			}
			break;
		case eSingleClipDirection::Down:
			mCurRow += (int)mClip.Direction;
			if (mCurRow == mClip.StartFrameX + mClip.Length)
			{
				if (mClip.IsLoop) { mCurRow = mClip.StartFrameX; }
				else
				{
					--mCurRow;
					mbIsEnd = true;
				}
			}
			break;
		case eSingleClipDirection::Repeat:
			++mCurCol;
			if (mCurCol == mClip.StartFrameX + mClip.Length)
			{
				++mCurRepeatCount;
				if (mCurRepeatCount == mClip.Repeat)
				{
					if (mClip.IsLoop)
					{
						mCurRepeatCount = 0;
						mCurCol = mClip.StartFrameX;
						mCurRow = mClip.StartFrameY + mCurRepeatCount;
					}
					else
					{
						--mCurCol;
						mbIsEnd = true;
					}
				}
				else
				{
					mCurCol = mClip.StartFrameX;
					mCurRow = mClip.StartFrameY + mCurRepeatCount;
				}
			}
			break;
		}
	}
}
