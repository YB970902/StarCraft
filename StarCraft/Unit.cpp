#include "stdafx.h"
#include "Unit.h"
#include "MultipleAnimation.h"

Unit::Unit()
	: GameObject::GameObject()
{
	Init();
}

Unit::~Unit()
{
}

void Unit::Init()
{
	mpTransform = static_cast<TransformComponent*>(GetComponent(eComponentTag::Transform));
	mpSprite = static_cast<SpriteComponent*>(AddComponent(new PairSpriteComponent(eBitmapTag::UNIT_MARINE_L, eBitmapTag::UNIT_MARINE_R, SpriteData::MARINE_FRAME_X, SpriteData::MARINE_FRAME_Y)));
	mpEffect = static_cast<EffectComponent*>(AddComponent(new EffectComponent(eEffectTag::COLOR_REPLACE)))->GetEffect();
	mpAnimator = static_cast<AnimatorComponent*>(AddComponent(new AnimatorComponent()));

	mpAnimator->AddMultipleAnimation((int)eAnimationTag::Idle, new MultipleAnimation(MarineIdle));
	mpAnimator->AddMultipleAnimation((int)eAnimationTag::Move, new MultipleAnimation(MarineMove));
	mpAnimator->AddMultipleAnimation((int)eAnimationTag::PrepairAttack, new MultipleAnimation(MarinePrepairAttack));
	mpAnimator->AddMultipleAnimation((int)eAnimationTag::PrepairIdle, new MultipleAnimation(MarinePrepairIdle));
	mpAnimator->AddMultipleAnimation((int)eAnimationTag::WaitAttack, new MultipleAnimation(MarineWaitAttack));
	mpAnimator->AddMultipleAnimation((int)eAnimationTag::Attack, new MultipleAnimation(MarineAttack));
	mpAnimator->ChangeAnimation((int)eAnimationTag::Idle);
	mpEffect->SetInput(0, mpSprite->GetBitmap());
	ChangeUnitColor(EFFECT_COLOR_RED);
}

void Unit::Release()
{
}

void Unit::Update()
{
	if (INPUT->IsOnceKeyDown('1')) { mpAnimator->ChangeAnimation((int)eAnimationTag::Idle); }
	if (INPUT->IsOnceKeyDown('2')) { mpAnimator->ChangeAnimation((int)eAnimationTag::Move); }
	if (INPUT->IsOnceKeyDown('3')) { mpAnimator->ChangeAnimation((int)eAnimationTag::PrepairAttack); }
	if (INPUT->IsOnceKeyDown('4')) { mpAnimator->ChangeAnimation((int)eAnimationTag::PrepairIdle); }
	if (INPUT->IsOnceKeyDown('5')) { mpAnimator->ChangeAnimation((int)eAnimationTag::WaitAttack); }
	if (INPUT->IsOnceKeyDown('6')) { mpAnimator->ChangeAnimation((int)eAnimationTag::Attack); }

	if (mbIsMove)
	{
		Vector2 dir = Vector2(mTargetPos - GetPosition()).Normalize();
		Vector2 moved = dir * (Fix)100 * DELTA_TIME;

		cout << "Moved X : " << moved.x << endl;
		cout << "Moved Y : " << moved.y << endl;

		mpTransform->SetPosition(GetPosition() + dir * (Fix)100 * DELTA_TIME);
		if (Vector2::GetDistance(GetPosition(), mTargetPos) < (Fix)0.001f)
		{
			mpAnimator->ChangeAnimation((int)eAnimationTag::Idle);
			mpTransform->SetPosition(mTargetPos);
			cout << "My X : " << GetPosition().x << ", My Y : " << GetPosition().y << endl;
			cout << "Target X : " << mTargetPos.x << ", Target Y : " << mTargetPos.y << endl;
			mbIsMove = false;
		}
	}
}

void Unit::SetTargetPosition(POINT pos)
{
	mTargetPos.x = pos.x;
	mTargetPos.y = pos.y;
	mbIsMove = true;
	mpAnimator->ChangeAnimation((int)eAnimationTag::Move);
	mpAnimator->Play();
}