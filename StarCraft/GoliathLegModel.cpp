#include "stdafx.h"
#include "GoliathLegModel.h"
#include "MultipleAnimation.h"

GoliathLegModel::GoliathLegModel()
{
	Init();
}

GoliathLegModel::~GoliathLegModel()
{
}

void GoliathLegModel::Init()
{
	mpSprite = static_cast<SpriteComponent*>(AddComponent(new PairSpriteComponent(
		eBitmapTag::UNIT_GOLIATH_LEG_L,
		eBitmapTag::UNIT_GOLIATH_LEG_R,
		SpriteData::GOLIATH_LEG_FRAME_X,
		SpriteData::GOLIATH_LEG_FRAME_Y,
		D2D1::Point2F(0.5f, 0.7f)
	)));
	mpEffect = static_cast<EffectComponent*>(AddComponent(new EffectComponent(eEffectTag::COLOR_REPLACE)))->GetEffect();
	mpAnimator = static_cast<AnimatorComponent*>(AddComponent(new AnimatorComponent()));

	mpAnimator->AddMultipleAnimation((int)eAnimationTag::Idle, new MultipleAnimation(GoliathLegIdle));
	mpAnimator->AddMultipleAnimation((int)eAnimationTag::Move, new MultipleAnimation(GoliathLegMove));
	mpAnimator->AddMultipleAnimation((int)eAnimationTag::PrepairAttack, new MultipleAnimation(GoliathLegIdle));
	mpAnimator->AddMultipleAnimation((int)eAnimationTag::PrepairIdle, new MultipleAnimation(GoliathLegIdle));
	mpAnimator->AddMultipleAnimation((int)eAnimationTag::WaitAttack, new MultipleAnimation(GoliathLegIdle));
	mpAnimator->AddMultipleAnimation((int)eAnimationTag::Attack, new MultipleAnimation(GoliathLegIdle));
	mpAnimator->ChangeAnimation((int)eAnimationTag::Idle);
	mpEffect->SetInput(0, mpSprite->GetBitmap());
	ChangeUnitColor(EFFECT_COLOR_BLACK);
}

void GoliathLegModel::Release()
{
}

void GoliathLegModel::Update()
{
}
