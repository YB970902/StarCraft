#pragma once
#include "UIGameObject.h"

class UICursor : public UIGameObject
{
private:
	SpriteComponent* mpSprite = nullptr;
	AnimatorComponent* mpAnimator = nullptr;

public:
	UICursor();
	virtual ~UICursor();

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;

	void ChangeAnimation(int tag);
};
