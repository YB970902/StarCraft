#pragma once
#include "Singleton.h"

class CameraManager : public Singleton<CameraManager>
{
private:
	Vector2 mPosition;
	float mZoom = 0.0f;
	D2D1_SIZE_F mSize;

public:
	void Init();

	bool IsInCamera(D2D_RECT_F rect);

	inline Vector2 GetPosition() { return mPosition; }
	inline D2D1_SIZE_F GetD2DPosition() { return D2D1_SIZE_F{ mPosition.x, mPosition.y }; }
	inline void SetPosition(Vector2 pos) { mPosition = pos; }
	inline void AddPosition(Vector2 add) { mPosition += add; }

	inline void SetZoom(float zoom) { mZoom = zoom; }
	inline void AddZoom(float zoom) { mZoom += zoom; }
	inline float GetZoom() { return mZoom; }

	inline D2D1_SIZE_F GetSize() { return mSize; }
};

