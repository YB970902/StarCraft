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

	bool mbIsMoved = false;
public:
	void Init();
	void Update();

	bool IsInCamera(D2D_RECT_F rect);

	inline Vector2 GetPosition() { return mPosition; }
	inline D2D1_SIZE_F GetD2DPosition() { return D2D1_SIZE_F{ mPosition.x, mPosition.y }; }
	inline void SetPosition(Vector2 pos) { mPosition = pos; mbIsMoved = true; }
	inline void AddPosition(Vector2 add) { mPosition += add; mbIsMoved = true; }

	inline void SetZoom(float zoom) { mZoom = zoom; }
	inline void AddZoom(float zoom) { mZoom += zoom; }
	inline float GetZoom() { return mZoom; }

	inline RECT GetCameraRect() { return mCameraRect; }

	inline D2D1_SIZE_F GetSize() { return mSize; }
};

