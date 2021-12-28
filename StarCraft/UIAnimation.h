#pragma once
#include "UIGameObject.h"

class UIAnimation : public UIGameObject
{
	SpriteComponent* mpSprite = nullptr;
	AnimatorComponent* mpAnimator = nullptr;
public:
	UIAnimation(eBitmapTag bitmapTag, const SingleAnimationClip& clip, D2D_POINT_2F anchor = D2D1::Point2F(0.5f, 0.5f));
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;

	inline Vector2 GetPosition() { return mpTransform->GetPosition(); }
	inline void SetPosition(const Vector2& pos) { mpTransform->SetPosition(pos); }
	inline void AddPosition(const Vector2& add) { mpTransform->AddPosition(add); }

	inline float GetSizeWidth() { return mpSprite->GetSizeWidth(); }
	inline float GetSizeHeight() { return mpSprite->GetSizeHeight(); }

	inline bool IsEnd() { return mpAnimator->IsEnd(); }

	inline bool SetIsRender(bool set) { mpRenderer->SetIsRender(set); }
};