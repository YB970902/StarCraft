#pragma once
#include "Singleton.h"

class CameraManager : public Singleton<CameraManager>
{
private:
	Vector2 mPosition;
	float mZoom = 0.0f;
	D2D1_SIZE_F mSize;

	POINT mLeftTop;
	POINT mRightBottom;

	RECT mCameraRect;

	POINT mMaxMovingSize = {};

	bool mbIsMoved = false;
public:
	void Init();
	void Update();

	bool IsInCamera(D2D_RECT_F rect);

	inline void SetMovingSize(POINT movingSize) { mMaxMovingSize = movingSize; }

	inline Vector2 GetPosition() { return mPosition; }
	inline D2D1_SIZE_F GetD2DPosition() { return D2D1_SIZE_F{ mPosition.x, mPosition.y }; }
	void SetPosition(Vector2 pos);
	void AddPosition(Vector2 add);

	inline void SetZoom(float zoom) { mZoom = zoom; }
	inline void AddZoom(float zoom) { mZoom += zoom; }
	inline float GetZoom() { return mZoom; }

	inline RECT GetCameraRect() { return mCameraRect; }

	inline D2D1_SIZE_F GetSize() { return mSize; }
};

