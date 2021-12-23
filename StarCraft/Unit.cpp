#include "stdafx.h"
#include "Unit.h"
#include "UnitModel.h"
#include "MultipleAnimation.h"
#include "UserManager.h"
#include "PhysicsManager.h"
#include "UnitManager.h"
#include "ParticleManager.h"
#include "SoundManager.h"

Unit::Unit(eTeamTag teamTag, UnitID ID, const float* arrColor)
	: GameObject::GameObject(), mTeamTag{ teamTag }, mID{ ID }, mArrColor{ arrColor }
{

}

Unit::~Unit()
{
}

void Unit::Init()
{

}

void Unit::Release()
{
}

void Unit::Update()
{

}

void Unit::SetTargetID(UnitID ID)
{
	UNIT->EndChase(mID, mTargetID);
	mTargetID = ID;

	if (mTargetID != UNIT_ID_NONE)
	{
		mTargetID = ID;
		UNIT->BeginChase(mID, mTargetID);
	}
}

bool Unit::AttackTarget()
{
	PARTICLE->CreateParticle(eParticleTag::ParticleAttack, Vector2(mTargetPos.x, mTargetPos.y));
	SOUND->Play(mAttackSound);
	return UNIT->Attack(mID, mTargetID);
}

void Unit::Move(const POINT& pos)
{
	SetTargetID(UNIT_ID_NONE);
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
		SetTargetID(UNIT_ID_NONE);
		return false;
	}
}

void Unit::UpdateTargetPos()
{
	UNIT->GetUnitPosition(mTargetID, mTargetPos);
}

void Unit::MoveAlertly(const POINT& pos)
{
	SetTargetID(UNIT_ID_NONE);
	mbIsHaveDestination = true;
	mDestination = pos;
	mpState->ChangeState(eStateTag::MoveAlertly);
}

void Unit::UpdateAngle()
{
	mpTransform->SetRotation(mpPathFind->GetAngle());
}

void Unit::StopFindPath()
{
	mpPathFind->Stop();
}

void Unit::LookAtTarget()
{
	float angle = atan2(-(mTargetPos.y - (int)GetPosition().y), mTargetPos.x - (int)GetPosition().x);
	mpTransform->SetRotation(RAD2DEG(angle));
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
	if (IsHaveTarget())
	{
		return mTargetPos;
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
		mpSprite->ChangeBitmap(mSelectedBitmap);
		mpEffect->SetValue((int)EffectData::eColorReplaceProperty::GROUP_COLOR, D2D_VECTOR_3F{ mArrColor[0], mArrColor[1], mArrColor[2] });
	}
	else
	{
		mpSprite->ChangeBitmap(eBitmapTag::NONE);
	}
}

bool Unit::OnDamaged(int attack)
{
	mCurHealth -= attack;
	if (mCurHealth <= 0)
	{
		mbIsDead = true;

		PARTICLE->CreateParticle(mDeadParticle, GetPosition());
		SOUND->Play(mDeadSound);

		SetTargetID(UNIT_ID_NONE);
		Notify(mID, eObserverMessage::Dead);
		UNIT->Dead(mID);

		return false;
	}
	return true;
}

void Unit::OnNotify(const UnitID& ID, const eObserverMessage& message)
{
	if (mbIsDead) { return; }
	switch (message)
	{
	case eObserverMessage::BeginChasing:
		AddObserver(UNIT->GetObserver(ID));
		break;
	case eObserverMessage::EndChasing:
		RemoveObserver(UNIT->GetObserver(ID));
		break;
	case eObserverMessage::Dead:
		UpdateTargetPos();
		RemoveObserver(UNIT->GetObserver(ID));
		SetTargetID(UNIT_ID_NONE);
		break;
	}
}