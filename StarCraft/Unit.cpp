#include "stdafx.h"
#include "Unit.h"
#include "UnitModel.h"
#include "MultipleAnimation.h"
#include "UserManager.h"
#include "PhysicsManager.h"
#include "UnitManager.h"
#include "ParticleManager.h"
#include "SoundManager.h"

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
	if (IsHaveTarget())
	{
		UNIT->GetUnitPosition(mTargetID, mTargetPos);
	}
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
	SOUND->Play(eSoundTag::MarineAttack);
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

bool Unit::OnDamaged(int attack)
{
	mCurHealth -= attack;
	if (mCurHealth <= 0)
	{
		PARTICLE->CreateParticle(eParticleTag::ParticleMarineDead, GetPosition());
		SOUND->Play(eSoundTag::MarineDead);

		SetTargetID(UNIT_ID_NONE);
		UNIT->Dead(mID);
		Notify(mID, eObserverMessage::Dead);
		mbIsDead = true;

		return false;
	}
	return true;
}

void Unit::OnNotify(const UnitID& ID, const eObserverMessage& message)
{
	switch (message)
	{
	case eObserverMessage::BeginChasing:
		AddObserver(UNIT->GetObserver(ID));
		break;
	case eObserverMessage::EndChasing:
		RemoveObserver(UNIT->GetObserver(ID));
		break;
	case eObserverMessage::Dead:
		RemoveObserver(UNIT->GetObserver(ID));
		SetTargetID(UNIT_ID_NONE);
		break;
	}
}