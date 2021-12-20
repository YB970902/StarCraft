#include "stdafx.h"
#include "Unit.h"
#include "UnitModel.h"
#include "MultipleAnimation.h"
#include "UserManager.h"

Unit::Unit(eTeamTag teamTag, UnitID ID)
	: GameObject::GameObject(), mTeamTag{ teamTag }, mID{ ID }
{
	Init();

	if (USER->GetTeamTag() == teamTag)
	{
		mArrColor = EFFECT_COLOR_GREEN;
	}
	else
	{
		mArrColor = EFFECT_COLOR_RED;
	}
}

Unit::~Unit()
{
}

void Unit::Init()
{
	mpSprite = static_cast<SpriteComponent*>(AddComponent(new SpriteComponent(eBitmapTag::SELECTED_CIRCLE_SMALL, SpriteData::SINGLE_FRAME_X, SpriteData::SINGLE_FRAME_Y)));
	mpEffect = static_cast<EffectComponent*>(AddComponent(new EffectComponent(eEffectTag::COLOR_REPLACE)))->GetEffect();
	mpPathFind = static_cast<PathFindComponent*>(AddComponent(new PathFindComponent(eUnitTileSize::Small)));
	AddComponent(new ColliderComponent(Vector2(0, -10), Vector2(16, 16), mTeamTag));
	mpEffect->SetInput(0, mpSprite->GetBitmap());
	SetIsSelected(false);
	mpModel = static_cast<UnitModel*>(AddChild(new UnitModel(), 0));
	mpModel->ChangeAnimation(eAnimationTag::Idle);
}

void Unit::Release()
{
}

void Unit::Update()
{
	if (mpPathFind->IsMoving())
	{
		if (mbIsMoving == false)
		{
			mbIsMoving = true;
			mpModel->ChangeAnimation(eAnimationTag::Move);
		}
		mpTransform->SetRotation((Fix)mpPathFind->GetAngle());
	}
	else
	{
		if (mbIsMoving)
		{
			mbIsMoving = false;
			mpModel->ChangeAnimation(eAnimationTag::Idle);
		}
	}

	if (INPUT->IsOnceKeyDown('Q'))
	{
		mpSprite->ChangeBitmap(eBitmapTag::NONE);
	}
}

void Unit::Move(POINT pos)
{
	mpPathFind->FindPath(Vector2(pos.x, pos.y));
}

void Unit::Attack(POINT pos)
{
}

void Unit::Stop()
{
	if (mpPathFind->IsMoving())
	{
		mpPathFind->Stop();
	}
}

void Unit::SetIsSelected(bool set)
{
	if (set)
	{
		mpSprite->ChangeBitmap(eBitmapTag::SELECTED_CIRCLE_SMALL);
		mpEffect->SetValue((int)EffectData::eColorReplaceProperty::GROUP_COLOR, D2D_VECTOR_3F{ mArrColor[0], mArrColor[1], mArrColor[2] });
	}
	else
	{
		mpSprite->ChangeBitmap(eBitmapTag::NONE);
	}
}