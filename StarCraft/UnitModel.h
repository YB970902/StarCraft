#pragma once
#include "GameObject.h"

class UnitModel : public GameObject
{
protected:
	SpriteComponent* mpSprite = nullptr;
	ID2D1Effect* mpEffect = nullptr;
	AnimatorComponent* mpAnimator = nullptr;

public:
	UnitModel();
	virtual ~UnitModel();

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;

	inline void ChangeUnitColor(const float* color) { mpEffect->SetValue((int)EffectData::eColorReplaceProperty::GROUP_COLOR, D2D_VECTOR_3F{ color[0], color[1], color[2] }); }
	void ChangeAnimation(eAnimationTag animTag);
};

