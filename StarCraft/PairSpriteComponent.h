#pragma once
#include "SpriteComponent.h"

class PairSpriteComponent : public SpriteComponent
{
protected:
	ID2D1Bitmap* mpLeftBitmap = nullptr;
	ID2D1Bitmap* mpRightBitmap = nullptr;
public:
	PairSpriteComponent(eBitmapTag leftTag, eBitmapTag rightTag, int frameX = 1, int frameY = 1, D2D1_POINT_2F anchor = D2D1_POINT_2F{ 0.5f, 0.5f });
	virtual ~PairSpriteComponent();
	virtual void Init(GameObject* pObject) override;
	virtual void Release() override;
	virtual void Update() override;

	inline ID2D1Bitmap* GetLeftBitmap() { return mpLeftBitmap; }
	inline ID2D1Bitmap* GetRightBitmap() { return mpRightBitmap; }

	virtual eComponentTag GetTag() override { return eComponentTag::PairSprite; }
};

