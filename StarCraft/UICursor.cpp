#include "stdafx.h"
#include "UICursor.h"
#include "SingleAnimation.h"

UICursor::UICursor()
{
	Init();
}

UICursor::~UICursor()
{
}

void UICursor::Init()
{
	mpSprite = static_cast<SpriteComponent*>(AddComponent(new SpriteComponent(
		eBitmapTag::UI_CURSOR,
		SpriteData::UI_CURSOR_X,
		SpriteData::UI_CURSOR_Y,
		D2D1::Point2F(0.5f, 0.5f)
	)));
	mpAnimator = static_cast<AnimatorComponent*>(AddComponent(new AnimatorComponent()));

	mpAnimator->AddSingleAnimation((int)eCursorState::Idle, new SingleAnimation(CursorIdle));
	mpAnimator->AddSingleAnimation((int)eCursorState::Drag, new SingleAnimation(CursorDrag));
	mpAnimator->AddSingleAnimation((int)eCursorState::OnGreen, new SingleAnimation(CursorOnGreen));
	mpAnimator->AddSingleAnimation((int)eCursorState::OnRed, new SingleAnimation(CursorOnRed));
	mpAnimator->AddSingleAnimation((int)eCursorState::OutDown, new SingleAnimation(CursorOutDown));
	mpAnimator->AddSingleAnimation((int)eCursorState::OutLeftDown, new SingleAnimation(CursorOutLeftDown));
	mpAnimator->AddSingleAnimation((int)eCursorState::OutRightDown, new SingleAnimation(CursorOutRightDown));
	mpAnimator->AddSingleAnimation((int)eCursorState::OutLeft, new SingleAnimation(CursorOutLeft));
	mpAnimator->AddSingleAnimation((int)eCursorState::OutRight, new SingleAnimation(CursorOutRight));
	mpAnimator->AddSingleAnimation((int)eCursorState::OutUp, new SingleAnimation(CursorOutUp));
	mpAnimator->AddSingleAnimation((int)eCursorState::OutUpLeft, new SingleAnimation(CursorOutUpLeft));
	mpAnimator->AddSingleAnimation((int)eCursorState::OutUpRight, new SingleAnimation(CursorOutUpRight));
	mpAnimator->AddSingleAnimation((int)eCursorState::PrepairGreen, new SingleAnimation(CursorPrepairGreen));
	mpAnimator->AddSingleAnimation((int)eCursorState::PrepairRed, new SingleAnimation(CursorPrepairRed));
	mpAnimator->AddSingleAnimation((int)eCursorState::PrepairYellow, new SingleAnimation(CursorPrepairYellow));
	mpAnimator->ChangeAnimation((int)eAnimationTag::Idle);
}

void UICursor::Release()
{
}

void UICursor::Update()
{
}

void UICursor::ChangeAnimation(int tag)
{
	mpAnimator->ChangeAnimation(tag);
}
