#include "stdafx.h"
#include "Goliath.h"
#include "UserManager.h"
#include "UnitManager.h"
#include "GoliathHeadModel.h"
#include "GoliathLegModel.h"

Goliath::Goliath(eTeamTag teamTag, UnitID ID, const float* arrColor)
	:Unit(teamTag, ID, arrColor)
{
	if (USER->GetTeamTag() == teamTag)
	{
		mArrColor = EFFECT_COLOR_GREEN;
	}
	else
	{
		mArrColor = EFFECT_COLOR_RED;
	}

	mAttackSound = eSoundTag::GoliathAttack;
	mDeadSound = eSoundTag::GoliathDead;
	mSelectedBitmap = eBitmapTag::SELECTED_CIRCLE_MIDIUM;
	mDeadParticle = eParticleTag::ParticleExplosion;
	mUnitTileSize = eUnitTileSize::Big;
	mUnitTag = eUnitTag::Goliath;
	Init();
	mpHeadModel->ChangeUnitColor(arrColor);
	mpLegModel->ChangeUnitColor(arrColor);
}

Goliath::~Goliath()
{
}

void Goliath::Init()
{
	mpSprite = static_cast<SpriteComponent*>(AddComponent(new SpriteComponent(eBitmapTag::SELECTED_CIRCLE_MIDIUM, SpriteData::SINGLE_FRAME_X, SpriteData::SINGLE_FRAME_Y)));
	mpEffect = static_cast<EffectComponent*>(AddComponent(new EffectComponent(eEffectTag::COLOR_REPLACE)))->GetEffect();
	mpPathFind = static_cast<PathFindComponent*>(AddComponent(new PathFindComponent(mUnitTileSize)));
	AddComponent(new ColliderComponent(Vector2(0, -10), Vector2(32, 32), mTeamTag));
	mpEffect->SetInput(0, mpSprite->GetBitmap());
	SetIsSelected(false);
	mpHeadModel = static_cast<UnitModel*>(AddChild(new GoliathHeadModel(), 1));
	mpLegModel = static_cast<UnitModel*>(AddChild(new GoliathLegModel(), 0));
	mpHeadModel->ChangeAnimation(eAnimationTag::Idle);
	mpLegModel->ChangeAnimation(eAnimationTag::Idle);
	mpState = static_cast<StateMachineComponent*>(AddComponent(new StateMachineComponent()));
}

void Goliath::Release()
{
}

void Goliath::Update()
{
}

void Goliath::ChangeAnimation(eAnimationTag animTag)
{
	mpHeadModel->ChangeAnimation(animTag);
	mpLegModel->ChangeAnimation(animTag);
}
