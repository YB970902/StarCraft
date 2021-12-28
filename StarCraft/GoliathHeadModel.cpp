#include "stdafx.h"
#include "GoliathHeadModel.h"
#include "MultipleAnimation.h"

GoliathHeadModel::GoliathHeadModel()
{
	Init();
}

GoliathHeadModel::~GoliathHeadModel()
{
}

void GoliathHeadModel::Init()
{
	mpSprite = static_cast<SpriteComponent*>(AddComponent(new PairSpriteComponent(
		eBitmapTag::UNIT_GOLIATH_HEAD_L,
		eBitmapTag::UNIT_GOLIATH_HEAD_R,
		SpriteData::GOLIATH_HEAD_FRAME_X,
		SpriteData::GOLIATH_HEAD_FRAME_Y,
		D2D1::Point2F(0.5f, 0.7f)
	)));
	mpEffect = static_cast<EffectComponent*>(AddComponent(new EffectComponent(eEffectTag::COLOR_REPLACE)))->GetEffect();
	mpAnimator = static_cast<AnimatorComponent*>(AddComponent(new AnimatorComponent()));

	mpAnimator->AddMultipleAnimation((int)eAnimationTag::Idle, new MultipleAnimation(GoliathHeadIdle));
	mpAnimator->AddMultipleAnimation((int)eAnimationTag::Move, new MultipleAnimation(GoliathHeadMove));
	mpAnimator->AddMultipleAnimation((int)eAnimationTag::PrepairAttack, new MultipleAnimation(GoliathHeadAttack));
	mpAnimator->AddMultipleAnimation((int)eAnimationTag::PrepairIdle, new MultipleAnimation(GoliathHeadIdle));
	mpAnimator->AddMultipleAnimation((int)eAnimationTag::WaitAttack, new MultipleAnimation(GoliathHeadIdle));
	mpAnimator->AddMultipleAnimation((int)eAnimationTag::Attack, new MultipleAnimation(GoliathHeadAttack));
	mpAnimator->ChangeAnimation((int)eAnimationTag::Idle);
	mpEffect->SetInput(0, mpSprite->GetBitmap());
	ChangeUnitColor(EFFECT_COLOR_BLACK);
}

void GoliathHeadModel::Release()
{
}

void GoliathHeadModel::Update()
{
}
