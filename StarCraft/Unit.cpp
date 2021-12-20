#include "stdafx.h"
#include "Unit.h"
#include "UnitModel.h"
#include "MultipleAnimation.h"
#include "UserManager.h"
#include "PhysicsManager.h"
#include "UnitManager.h"

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
	mpState = static_cast<StateMachineComponent*>(AddComponent(new StateMachineComponent()));
}

void Unit::Release()
{
}

void Unit::Update()
{
	if (INPUT->IsOnceKeyDown('Q'))
	{
		mpSprite->ChangeBitmap(eBitmapTag::NONE);
	}
}

void Unit::Move(const POINT& pos)
{
	mbIsHaveDestination = true;
	mDestination = pos;
	mpState->ChangeState(eStateTag::Move);
}

void Unit::ChaseTarget(UnitID ID)
{
	SetTargetID(ID);
	mpState->ChangeState(eStateTag::Chase);
}

bool Unit::FindCloserEnemy()
{
	UnitID targetID = UNIT_ID_NONE;
	if (PHYSICS->GetNearEnemyUnit(mTeamTag, GetPosition(), mVisionRange, &targetID))
	{
		SetTargetID(targetID);
		return true;
	}
	else
	{
		return false;
	}
}

void Unit::MoveAlertly(const POINT& pos)
{
	mbIsHaveDestination = true;
	mDestination = pos;
	mpState->ChangeState(eStateTag::MoveAlertly);
}

void Unit::UpdateAngle()
{
	mpTransform->SetRotation(mpPathFind->GetAngle());
}

void Unit::ChangeAnimation(eAnimationTag animTag)
{
	mpModel->ChangeAnimation(animTag);
}

void Unit::StopFindPath()
{
	mpPathFind->Stop();
}

void Unit::LookAtTarget()
{
	POINT targetPos;
	if (UNIT->GetUnitPosition(mTargetID, targetPos))
	{
		float angle = atan2(-(targetPos.y - (int)GetPosition().y), targetPos.x - (int)GetPosition().x);
		mpTransform->SetRotation(RAD2DEG(angle));
	}
}

void Unit::FindPath(const POINT& pos)
{
	mpPathFind->FindPath(Vector2(pos.x, pos.y));
}

bool Unit::IsMoving()
{
	return mpPathFind->IsMoving();
}

POINT Unit::GetTargetPosition()
{
	POINT result;
	if (UNIT->GetUnitPosition(mTargetID, result))
	{
		return result;
	}
	else
	{
		POINT{ GetPosition().x, GetPosition().y };
	}
}

bool Unit::IsArrived()
{
	return mpPathFind->IsArrived();
}

int Unit::GetDistanceToTarget()
{
	return PHYSICS->GetDistance(this, mTargetID);
}

void Unit::Stop()
{
	mpState->ChangeState(eStateTag::Idle);
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