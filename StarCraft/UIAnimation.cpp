#include "stdafx.h"
#include "UIAnimation.h"
#include "SingleAnimation.h"

UIAnimation::UIAnimation(eBitmapTag bitmapTag, const SingleAnimationClip& clip, D2D_POINT_2F anchor)
{
	if (clip.Direction == eSingleClipDirection::Repeat)
	{
		mpSprite = static_cast<SpriteComponent*>(AddComponent(new SpriteComponent(bitmapTag, clip.Length, clip.Repeat, anchor)));
	}
	else
	{
		mpSprite = static_cast<SpriteComponent*>(AddComponent(new SpriteComponent(bitmapTag, clip.Length, 1, anchor)));
	}
	mpAnimator = static_cast<AnimatorComponent*>(AddComponent(new AnimatorComponent()));
	mpAnimator->AddSingleAnimation((int)eAnimationTag::Idle, new SingleAnimation(clip));
	Init();
}

void UIAnimation::Init()
{
	mpAnimator->ChangeAnimation((int)eAnimationTag::Idle);
	mpAnimator->Play();
}

void UIAnimation::Release()
{
}

void UIAnimation::Update()
{
}
