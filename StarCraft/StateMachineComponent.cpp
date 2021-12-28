#include "stdafx.h"
#include "StateMachineComponent.h"
#include "Unit.h"
#include "PhysicsManager.h"

StateMachineComponent::StateMachineComponent()
{
	
}

StateMachineComponent::~StateMachineComponent()
{
}

void StateMachineComponent::Init(GameObject* pObject)
{
	mpGameObject = pObject;
	mpUnit = static_cast<Unit*>(pObject);
	mMapState[eStateTag::Idle] = new IdleState();
	mMapState[eStateTag::Move] = new MoveState();
	mMapState[eStateTag::MoveAlertly] = new MoveAlertlyState();
	mMapState[eStateTag::Attack] = new AttackState();
	mMapState[eStateTag::Chase] = new ChaseState();

	for (auto it = mMapState.begin(); it != mMapState.end(); ++it)
	{
		it->second->Init(mpUnit, this);
	}
}

void StateMachineComponent::Release()
{
	mpCurState = nullptr;
	for (auto it = mMapState.begin(); it != mMapState.end(); )
	{
		delete it->second;
		it = mMapState.erase(it);
	}
}

void StateMachineComponent::Update()
{
	if (mpCurState && mpUnit->IsAlive())
	{
		mpCurState->Update();
	}
}

void StateMachineComponent::ChangeState(eStateTag tag)
{
	if (mpCurState)
	{
		if (mCurStateTag == eStateTag::Attack)
		{
			mbIsHaveNextState = true;
			mNextStateTag = tag;
			return;
		}
		mpCurState->Exit();
	}
	mpCurState = mMapState[tag];
	mCurStateTag = tag;
	mpCurState->Enter();
}

void StateMachineComponent::ChangeNextState()
{
	if (mbIsHaveNextState)
	{
		mbIsHaveNextState = false;
		if (mpCurState) { mpCurState->Exit(); }
		mCurStateTag = mNextStateTag;
		mpCurState = mMapState[mNextStateTag];
		mpCurState->Enter();
	}
	else
	{
		int i = 0;
	}
}

eComponentTag StateMachineComponent::GetTag()
{
	return eComponentTag::StateMachine;
}

void State::Init(Unit* pUnit, StateMachineComponent* pStateMachine)
{
	mpUnit = pUnit;
	mpStateMachine = pStateMachine;
}

void IdleState::Enter()
{
	mElapsedTime = 0.0f;
	mpUnit->StopFindPath();
	mpUnit->ChangeAnimation(eAnimationTag::Idle);
}

void IdleState::Update()
{
	mElapsedTime += DELTA_TIME;
	if (mElapsedTime >= mDurationTime)
	{
		mElapsedTime -= mDurationTime;
		if (mpUnit->FindCloserEnemy())
		{
			mpUnit->UpdateTargetPos();
			mpStateMachine->ChangeState(eStateTag::Chase);
			return;
		}
	}
}

void MoveState::Enter()
{
	mbIsMoving = true;
	mpUnit->ChangeAnimation(eAnimationTag::Move);
	mpUnit->FindPath(mpUnit->GetDestination());
}

void MoveState::Exit()
{
	mpUnit->StopFindPath();
}

void MoveState::Update()
{
	mpUnit->UpdateAngle();

	if (mpUnit->IsMoving())
	{
		if (mbIsMoving == false)
		{
			mbIsMoving = true;
			mpUnit->ChangeAnimation(eAnimationTag::Move);
		}
	}
	else
	{
		if (mbIsMoving)
		{
			mbIsMoving = false;
			mpUnit->ChangeAnimation(eAnimationTag::Idle);
		}
	}

	if(mpUnit->IsArrived())
	{
		mpUnit->SetIsHaveDestination(false);
		mpStateMachine->ChangeState(eStateTag::Idle);
		return;
	}
}

void AttackState::Init(Unit* pUnit, StateMachineComponent* pStateMachine)
{
	mpUnit = pUnit;
	mpStateMachine = pStateMachine;

	mDurationTime = mpUnit->GetAttackSpeed();
}

void AttackState::Enter()
{
	mElapsedTime = 0;
	mElapsedFlashTime = 0;
	mCurFireCount = 0;
	mpUnit->ChangeAnimation(eAnimationTag::WaitAttack);
	if (mpUnit->AttackTarget() == false)
	{
		if (mpUnit->IsHaveDestination())
		{
			mpStateMachine->ChangeState(eStateTag::MoveAlertly);
		}
		else
		{
			mpStateMachine->ChangeState(eStateTag::Idle);
		}
		mpStateMachine->ChangeNextState();
	}
	else
	{
		mpUnit->UpdateTargetPos();
		mpUnit->LookAtTarget();
	}
}

void AttackState::Exit()
{
}

void AttackState::Update()
{
	mpUnit->LookAtTarget();

	if (mbIsPrepare)
	{
		mElapsedTime += DELTA_TIME;
		if (mElapsedTime >= mDurationTime)
		{
			mElapsedTime -= mDurationTime;
			mbIsPrepare = false;
			mpUnit->ChangeAnimation(eAnimationTag::Attack);
		}
	}
	else
	{
		mElapsedFlashTime += DELTA_TIME;
		if (mElapsedFlashTime >= mDurationFlashTime)
		{
			mElapsedFlashTime -= mDurationFlashTime;
			mbIsPrepare = true;
			mpUnit->ChangeAnimation(eAnimationTag::WaitAttack);
			++mCurFireCount;
			if (mCurFireCount == mMaxFireCount)
			{
				if (mpStateMachine->IsHaveNextState())
				{
					mpStateMachine->ChangeNextState();
				}
				else
				{
					if (mpUnit->IsHaveTarget())
					{
						if (mpUnit->GetDistanceToTarget() > mpUnit->GetAttackRange())
						{
							mpStateMachine->ChangeState(eStateTag::Chase);
							mpStateMachine->ChangeNextState();
						}
						else
						{
							Enter();
						}
					}
					else
					{
						if (mpUnit->IsHaveDestination())
						{
							mpStateMachine->ChangeState(eStateTag::MoveAlertly);
						}
						else
						{
							mpStateMachine->ChangeState(eStateTag::Idle);
						}
						mpStateMachine->ChangeNextState();
					}
				}
			}
		}
	}
}

void ChaseState::Enter()
{
	mElapsedFindPathTime = 0;
	mbIsMoving = true;
	mpUnit->ChangeAnimation(eAnimationTag::Move);
}

void ChaseState::Exit()
{
	mpUnit->StopFindPath();
}

void ChaseState::Update()
{
	mpUnit->UpdateAngle();

	if (mpUnit->IsMoving())
	{
		if (mbIsMoving == false)
		{
			mbIsMoving = true;
			mpUnit->ChangeAnimation(eAnimationTag::Move);
		}
	}
	else
	{
		if (mbIsMoving)
		{
			mbIsMoving = false;
			mpUnit->ChangeAnimation(eAnimationTag::Idle);
		}
	}

	if (mpUnit->IsHaveTarget() == false)
	{
		if (mpUnit->IsHaveDestination())
		{
			mpStateMachine->ChangeState(eStateTag::MoveAlertly);
		}
		else
		{
			mpStateMachine->ChangeState(eStateTag::Idle);
		}
		return;
	}

	mElapsedFindPathTime += DELTA_TIME;
	if (mElapsedFindPathTime >= mDurationFindPathTime)
	{
		mElapsedFindPathTime -= mDurationFindPathTime;
		mpUnit->UpdateTargetPos();
		mpUnit->FindPath(mpUnit->GetTargetPosition());
	}
	else if (mpUnit->IsArrived())
	{
		mpUnit->UpdateTargetPos();
		mpUnit->FindPath(mpUnit->GetTargetPosition());
	}

	if (mpUnit->GetDistanceToTarget() <= mpUnit->GetAttackRange())
	{
		mpUnit->UpdateTargetPos();
		mpStateMachine->ChangeState(eStateTag::Attack);
	}
}

void MoveAlertlyState::Enter()
{
	mElapsedDetectTime = 0;
	if (mpUnit->IsHaveDestination())
	{
		mpUnit->FindPath(mpUnit->GetDestination());
		mbIsMoving = true;
		mpUnit->ChangeAnimation(eAnimationTag::Move);
	}
	else
	{
		mpStateMachine->ChangeState(eStateTag::Idle);
	}
}

void MoveAlertlyState::Exit()
{
	mpUnit->StopFindPath();
}

void MoveAlertlyState::Update()
{
	mpUnit->UpdateAngle();

	if (mpUnit->IsMoving())
	{
		if (mbIsMoving == false)
		{
			mbIsMoving = true;
			mpUnit->ChangeAnimation(eAnimationTag::Move);
		}
	}
	else
	{
		if (mbIsMoving)
		{
			mbIsMoving = false;
			mpUnit->ChangeAnimation(eAnimationTag::Idle);
		}
	}

	if (mpUnit->IsArrived())
	{
		mpUnit->SetIsHaveDestination(false);
		mpStateMachine->ChangeState(eStateTag::Idle);
		return;
	}

	mElapsedDetectTime += DELTA_TIME;
	if (mElapsedDetectTime >= mDurationDetectTime)
	{
		mElapsedDetectTime -= mDurationDetectTime;
		if (mpUnit->FindCloserEnemy())
		{
			mpStateMachine->ChangeState(eStateTag::Chase);
			return;
		}
	}
}
