#include "stdafx.h"
#include "Marine.h"
#include "UserManager.h"
#include "UnitManager.h"
#include "MarineModel.h"

Marine::Marine(eTeamTag teamTag, UnitID ID, const float* arrColor)
	: Unit(teamTag, ID, arrColor)
{
	if (USER->GetTeamTag() == teamTag)
	{
		mArrColor = EFFECT_COLOR_GREEN;
	}
	else
	{
		mArrColor = EFFECT_COLOR_RED;
	}

	mAttackSound = eSoundTag::MarineAttack;
	mDeadSound = eSoundTag::MarineDead;
	mSelectedBitmap = eBitmapTag::SELECTED_CIRCLE_SMALL;
	mDeadParticle = eParticleTag::ParticleMarineDead;
	Init();
	mpModel->ChangeUnitColor(arrColor);
}

Marine::~Marine()
{
}

void Marine::Init()
{
	mpSprite = static_cast<SpriteComponent*>(AddComponent(new SpriteComponent(eBitmapTag::SELECTED_CIRCLE_SMALL, SpriteData::SINGLE_FRAME_X, SpriteData::SINGLE_FRAME_Y)));
	mpEffect = static_cast<EffectComponent*>(AddComponent(new EffectComponent(eEffectTag::COLOR_REPLACE)))->GetEffect();
	mpPathFind = static_cast<PathFindComponent*>(AddComponent(new PathFindComponent(eUnitTileSize::Small)));
	AddComponent(new ColliderComponent(Vector2(0, -10), Vector2(16, 16), mTeamTag));
	mpEffect->SetInput(0, mpSprite->GetBitmap());
	SetIsSelected(false);
	mpModel = static_cast<MarineModel*>(AddChild(new MarineModel(), 0));
	mpModel->ChangeAnimation(eAnimationTag::Idle);
	mpState = static_cast<StateMachineComponent*>(AddComponent(new StateMachineComponent()));
}

void Marine::Release()
{
}

void Marine::Update()
{
	if (IsHaveTarget())
	{
		UNIT->GetUnitPosition(mTargetID, mTargetPos);
	}
}

void Marine::ChangeAnimation(eAnimationTag animTag)
{
	mpModel->ChangeAnimation(animTag);
}
