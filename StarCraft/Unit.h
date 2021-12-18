#pragma once
#include "GameObject.h"

class Unit : public GameObject
{
private:
	SpriteComponent* mpSprite = nullptr;
	ID2D1Effect* mpEffect = nullptr;
	PathFindComponent* mpPathFind = nullptr;

	Vector2 mTargetPos = {0, 0};
	bool mbIsMove = false;

	eTeamTag mTeamTag;

protected:
	Vector2 mColliderPos = { 0, 0 };

public:
	Unit(eTeamTag teamTag);
	virtual ~Unit();
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;

	inline Vector2 GetPosition() { return mpTransform->GetPosition(); }
	inline Vector2 GetColliderPosition() { return mpTransform->GetPosition() + mColliderPos; }
	inline void SetPosition(Fix x, Fix y) { mpTransform->SetPosition(x, y); if (mpPathFind) { mpPathFind->SetStop(); } }
	inline void SetRotation(Fix angle) { mpTransform->SetRotation(angle); }

	inline void ChangeCircleColor(const float* color) { mpEffect->SetValue((int)EffectData::eColorReplaceProperty::GROUP_COLOR, D2D_VECTOR_3F{ color[0], color[1], color[2] }); }

	void SetTargetPosition(POINT pos);

	void FindPath(POINT pos);

	inline eTeamTag GetTeamTag() { return mTeamTag; }
};