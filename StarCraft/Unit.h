#pragma once
#include "GameObject.h"

class Unit : public GameObject
{
	TransformComponent* mpTransform = nullptr;
	SpriteComponent* mpSprite = nullptr;
	ID2D1Effect* mpEffect = nullptr;
	AnimatorComponent* mpAnimator = nullptr;

	Vector2 mTargetPos = {0, 0};
	bool mbIsMove = false;
public:
	Unit();
	virtual ~Unit();
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;

	inline void ChangeUnitColor(const float* color) { mpEffect->SetValue((int)EffectData::eColorReplaceProperty::GROUP_COLOR, D2D_VECTOR_3F{ color[0], color[1], color[2] }); }

	inline Vector2 GetPosition() { return mpTransform->GetPosition(); }
	inline void SetPosition(Fix x, Fix y) { mpTransform->SetPosition(x, y); }
	inline void SetRotation(Fix angle) { mpTransform->SetRotation(angle); }

	void SetTargetPosition(POINT pos);
};