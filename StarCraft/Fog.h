#pragma once
#include "GameObject.h"

class Fog : public GameObject
{
private:
	SpriteComponent* mpSprite = nullptr;
	ID2D1Effect* mpEffect = nullptr;
public:
	Fog();
	virtual ~Fog();

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;

	void SetFrameX(int frame);
};

