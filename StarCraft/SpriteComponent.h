#pragma once
#include "Component.h"
#include "SpriteData.h"

class SpriteComponent : public Component
{
protected:
	ID2D1Bitmap* mpBitmap = nullptr;
	D2D1_SIZE_F mSize = { 0, 0 };
	D2D1_SIZE_F mMaxSize = { 0, 0 };
	D2D1_POINT_2F mOffset = { 0, 0 };
	D2D1_POINT_2F mAnchor = { 0, 0 };

	int mCurFrameX = 0;
	int mCurFrameY = 0;
	int mMaxFrameX = 0;
	int mMaxFrameY = 0;

	int mOrder = 0;

public:
	SpriteComponent(eBitmapTag tag, int frameX = 1, int frameY = 1, D2D1_POINT_2F anchor = D2D1_POINT_2F{ 0.5f, 0.5f });
	SpriteComponent(const SpriteComponent& other) = delete;
	virtual ~SpriteComponent();
	virtual void Init(GameObject* pObject) override;
	virtual void Release() override;
	virtual void Update() override;

	void SetCurFrameX(int x);
	void SetCurFrameY(int y);
	void SetCurFrame(int x, int y);

	inline int GetMaxFrameX() { return mMaxFrameX; }
	inline int GetMaxFrameY() { return mMaxFrameY; }
	void SetMaxFrame(int x, int y);

	inline void SetAnchor(D2D1_POINT_2F anchor) { mAnchor = anchor; }
	inline int GetOrder() { return mOrder; }
	inline void SetOrder(int order) { mOrder = order; }

	inline float GetAnchorX() { return -mSize.width * mAnchor.x; }
	inline float GetAnchorY() { return -mSize.height * mAnchor.y; }
	inline float GetOffsetX() { return mOffset.x; }
	inline float GetOffsetY() { return mOffset.y; }
	inline float GetSizeWidth() { return mSize.width; }
	inline void SetSizeWidth(float width) { mSize.width = width; }
	inline float GetSizeHeight() { return mSize.height; }
	inline D2D_SIZE_F GetD2DSize() { return mSize; }
	inline D2D_RECT_F GetSourceRect() { return D2D_RECT_F{ mOffset.x, mOffset.y, mOffset.x + mSize.width, mOffset.y + mSize.height }; }

	inline ID2D1Bitmap* GetBitmap() { return mpBitmap; }
	inline void SetBitmap(ID2D1Bitmap* pBitmap) { mpBitmap = pBitmap; }

	void ChangeBitmap(eBitmapTag tag);
};