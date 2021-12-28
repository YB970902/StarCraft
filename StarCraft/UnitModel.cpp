#include "stdafx.h"
#include "UnitModel.h"
#include "MultipleAnimation.h"

UnitModel::UnitModel()
	: GameObject::GameObject()
{
	//Init();
}

UnitModel::~UnitModel()
{
}

void UnitModel::Init()
{
	//mpSprite = static_cast<SpriteComponent*>(AddComponent(new PairSpriteComponent(
	//	eBitmapTag::UNIT_MARINE_L,
	//	eBitmapTag::UNIT_MARINE_R,
	//	SpriteData::MARINE_FRAME_X,
	//	SpriteData::MARINE_FRAME_Y,
	//	D2D1::Point2F(0.5f, 0.7f)
	//)));
	//mpEffect = static_cast<EffectComponent*>(AddComponent(new EffectComponent(eEffectTag::COLOR_REPLACE)))->GetEffect();
	//mpAnimator = static_cast<AnimatorComponent*>(AddComponent(new AnimatorComponent()));
	//
	//mpAnimator->AddMultipleAnimation((int)eAnimationTag::Idle, new MultipleAnimation(MarineIdle));
	//mpAnimator->AddMultipleAnimation((int)eAnimationTag::Move, new MultipleAnimation(MarineMove));
	//mpAnimator->AddMultipleAnimation((int)eAnimationTag::PrepairAttack, new MultipleAnimation(MarinePrepairAttack));
	//mpAnimator->AddMultipleAnimation((int)eAnimationTag::PrepairIdle, new MultipleAnimation(MarinePrepairIdle));
	//mpAnimator->AddMultipleAnimation((int)eAnimationTag::WaitAttack, new MultipleAnimation(MarineWaitAttack));
	//mpAnimator->AddMultipleAnimation((int)eAnimationTag::Attack, new MultipleAnimation(MarineAttack));
	//mpAnimator->ChangeAnimation((int)eAnimationTag::Idle);
	//mpEffect->SetInput(0, mpSprite->GetBitmap());
	//ChangeUnitColor(EFFECT_COLOR_RED);
}

void UnitModel::Release()
{
}

void UnitModel::Update()
{
	
}

void UnitModel::ChangeAnimation(eAnimationTag animTag)
{
	mpAnimator->ChangeAnimation((int)animTag);
}
