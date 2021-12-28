#pragma once
#include "Component.h"

class Unit;
class State;
class StateMachineComponent : public Component
{
private:
	unordered_map<eStateTag, State*> mMapState;
	eStateTag mCurStateTag = eStateTag::None;
	State* mpCurState = nullptr;
	Unit* mpUnit = nullptr;

	bool mbIsHaveNextState = false;
	eStateTag mNextStateTag = eStateTag::None;

public:
	StateMachineComponent();
	virtual ~StateMachineComponent();
	virtual void Init(GameObject* pObject) override;
	virtual void Release() override;
	virtual void Update() override;

	void ChangeState(eStateTag tag);
	void ChangeNextState();

	virtual eComponentTag GetTag() override;

	inline bool IsHaveNextState() { return mbIsHaveNextState; }
	inline eStateTag GetNextStateTag() { return mNextStateTag; }
};

class State
{
protected:
	Unit* mpUnit = nullptr;
	StateMachineComponent* mpStateMachine = nullptr;
public:
	virtual void Init(Unit* pUnit, StateMachineComponent* pStateMachine);
	virtual void Enter() abstract;
	virtual void Exit() abstract;
	virtual void Update() abstract;
};

class IdleState : public State
{
private:
	Fix mElapsedTime = 0.0f;
	const Fix mDurationTime = 0.5f;
public:
	virtual void Enter() override;
	virtual void Exit() override { }
	virtual void Update() override;
};

class MoveState : public State
{
private:
	bool mbIsMoving = false;
public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update() override;
};

class MoveAlertlyState : public State
{
private:
	Fix mElapsedDetectTime = 0;
	Fix mDurationDetectTime = 0.5f;

	bool mbIsMoving = false;
public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update() override;
};

class ChaseState : public State
{
private:
	Fix mDurationFindPathTime = 2;
	Fix mElapsedFindPathTime = 0;

	bool mbIsMoving = true;
public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update() override;
};

class AttackState : public State
{
private:
	Fix mElapsedTime = 0;
	Fix mDurationTime = 0;
	Fix mElapsedFlashTime = 0;
	Fix mDurationFlashTime = 0.05f;

	int mCurFireCount = 0;
	int mMaxFireCount = 2;

	bool mbIsPrepare = true;
public:
	virtual void Init(Unit* pUnit, StateMachineComponent* pStateMachine) override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update() override;
};