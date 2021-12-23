#pragma once
#include "GameObject.h"
#include "IObserver.h"
#include "Subject.h"

class UnitModel;
class UnitManager;
class Unit : public GameObject, public IObserver, public Subject
{
	friend UnitManager;
protected:
	UnitID mID = UNIT_ID_NONE;
	eTeamTag mTeamTag;
	const float* mArrColor;

	SpriteComponent* mpSprite = nullptr;
	ID2D1Effect* mpEffect = nullptr;
	PathFindComponent* mpPathFind = nullptr;
	StateMachineComponent* mpState = nullptr;
	eBitmapTag mSelectedBitmap;

	eSoundTag mAttackSound;
	eSoundTag mDeadSound;
	eParticleTag mDeadParticle;

protected:
	int mMaxHealth = 10;
	int mCurHealth = 10;
	int mAttack = 1;
	int mAttackRange = 200;
	int mVisionRange = 400;
	Fix mAttackSpeed = 0.25f;
	UnitID mTargetID = UNIT_ID_NONE;
	POINT mTargetPos;
	bool mbIsDead = false;

	bool mbIsHaveDestination = false;
	POINT mDestination;

public:
	Unit(eTeamTag teamTag, UnitID ID, const float* arrColor);
	virtual ~Unit();
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;

	inline Vector2 GetPosition() { return mpTransform->GetPosition(); }
	inline void SetPosition(Fix x, Fix y) { mpTransform->SetPosition(x, y); }
	inline void SetRotation(Fix angle) { mpTransform->SetRotation(angle); }


	inline eTeamTag GetTeamTag() { return mTeamTag; }
	inline UnitID GetUnitID() { return mID; }
	void SetTargetID(UnitID ID);
	bool AttackTarget();

	void UpdateAngle();
	void UpdateTargetPos();
	void LookAtTarget();
	void FindPath(const POINT& pos);
	void StopFindPath();

	bool FindCloserEnemy();
	inline bool IsHaveTarget() { return (mTargetID != UNIT_ID_NONE); }
	inline int GetAttack() { return mAttack; }
	inline int GetAttackRange() { return mAttackRange; }
	inline int GetVisionRange() { return mVisionRange; }
	inline Fix GetAttackSpeed() { return mAttackSpeed; }
	inline bool IsDead() { return mbIsDead; }
	inline bool IsAlive() { return !mbIsDead; }
	POINT GetTargetPosition();
	int GetDistanceToTarget();

	inline bool IsHaveDestination() { return mbIsHaveDestination; }
	inline void SetIsHaveDestination(bool set) { mbIsHaveDestination = set; }
	inline POINT GetDestination() { return mDestination; }

	bool IsMoving();
	bool IsArrived();

	virtual void ChangeAnimation(eAnimationTag animTag) {}

protected:
	void SetIsSelected(bool set);
	void Move(const POINT& pos);
	void MoveAlertly(const POINT& pos);
	void ChaseTarget(UnitID ID);

	void Stop();

	bool OnDamaged(int attack);

	virtual void OnNotify(const UnitID& ID, const eObserverMessage& message) override;

};