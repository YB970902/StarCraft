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

	UnitModel* mpModel = nullptr;

	bool mbIsMoving = false;

protected:
	int mAttackRange = 0;
	int mVisionRange = 0;
	UnitID mTargetID = UNIT_ID_NONE;

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

	void Move(POINT pos);
	void Attack(POINT pos);
	void Stop();

	bool IsHaveTarget() { return (mTargetID != UNIT_ID_NONE); }
	int GetAttackRange() { return mAttackRange; }
	int GetVisionRange() { return mVisionRange; }
};