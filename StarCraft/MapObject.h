#pragma once
#include "GameObject.h"

class MapObject : public GameObject
{
private:
	SpriteComponent* mpSprite = nullptr;
	EffectComponent* mpEffect = nullptr;

	eBitmapTag mBitmapTag = eBitmapTag::NONE;

public:
	MapObject(eBitmapTag bitmapTag);
	virtual ~MapObject();

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;

	inline void SetMaxFrame(int x, int y) { if (mpSprite) { mpSprite->SetMaxFrame(x, y); } }
	inline void ChangeSprite(eBitmapTag bitmapTag) { if (mpSprite) { mpSprite->ChangeBitmap(bitmapTag); } mBitmapTag = bitmapTag; }
	void ChangeColor(const float* arrColor);

	inline float GetWidth() { if (mpSprite) { return mpSprite->GetSizeWidth(); } return 0; }
	inline float GetHeight() { if (mpSprite) { return mpSprite->GetSizeHeight(); } return 0; }

	inline int GetMaxFrameX() { if (mpSprite) { return mpSprite->GetMaxFrameX(); } return 0; }
	inline int GetMaxFrameY() { if (mpSprite) { return mpSprite->GetMaxFrameY(); } return 0; }

	inline eBitmapTag GetBitmapTag() { return mBitmapTag; }

	virtual void SetPosition(const Vector2& pos) { mpTransform->SetPosition(pos); }
};