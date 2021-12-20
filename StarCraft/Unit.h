#pragma once
#include "GameObject.h"

class UnitModel;
class Unit : public GameObject
{
private:
	UnitID mID = UNIT_ID_NONE;
	eTeamTag mTeamTag;
	const float* mArrColor;

	SpriteComponent* mpSprite = nullptr;
	ID2D1Effect* mpEffect = nullptr;
	PathFindComponent* mpPathFind = nullptr;
	StateMachineComponent* mpState = nullptr;

	UnitModel* mpModel = nullptr;

protected:
	int mMaxHealth = 10;
	int mCurHealth = 10;
	int mAttack = 1;
	int mAttackRange = 200;
	int mVisionRange = 300;
	Fix mAttackSpeed = 0.25f;
	UnitID mTargetID = UNIT_ID_NONE;

	bool mbIsHaveDestination = false;
	POINT mDestination;

public:
	Unit(eTeamTag teamTag, UnitID ID);
	virtual ~Unit();
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;

	inline Vector2 GetPosition() { return mpTransform->GetPosition(); }
	inline void SetPosition(Fix x, Fix y) { mpTransform->SetPosition(x, y); }
	inline void SetRotation(Fix angle) { mpTransform->SetRotation(angle); }

	void SetIsSelected(bool set);

	inline eTeamTag GetTeamTag() { return mTeamTag; }
	inline UnitID GetUnitID() { return mID; }
	inline void SetTargetID(UnitID ID) { mTargetID = ID; }

	void Move(const POINT& pos);
	void MoveAlertly(const POINT& pos);
	void ChaseTarget(UnitID ID);
	void Stop();

	void UpdateAngle();
	void LookAtTarget();
	void FindPath(const POINT& pos);
	void StopFindPath();

	bool FindCloserEnemy();
	inline bool IsHaveTarget() { return (mTargetID != UNIT_ID_NONE); }
	inline int GetAttackRange() { return mAttackRange; }
	inline int GetVisionRange() { return mVisionRange; }
	inline Fix GetAttackSpeed() { return mAttackSpeed; }
	POINT GetTargetPosition();
	int GetDistanceToTarget();

	inline bool IsHaveDestination() { return mbIsHaveDestination; }
	inline void SetIsHaveDestination(bool set) { mbIsHaveDestination = set; }
	inline POINT GetDestination() { return mDestination; }

	bool IsMoving();
	bool IsArrived();

	void ChangeAnimation(eAnimationTag animTag);
};