#pragma once
#include "RenderConfig.h"

class Gizmo
{
protected:
	Vector2 mPosition;
	ID2D1SolidColorBrush* mpBrush = nullptr;
	bool mbIsActive = true;
public:
	Gizmo(Vector2 position, ID2D1SolidColorBrush* pBrush);
	virtual ~Gizmo() = default;
	virtual void Render(ID2D1DeviceContext2* pD2DContext) { };

	inline void SetPosition(Vector2 set) { mPosition = set; }
	inline Vector2 GetPosition() { return mPosition; }
	inline void SetColor(D2D1::ColorF color) { if (mpBrush) { mpBrush->SetColor(color); } }

	inline bool IsActive() { return mbIsActive; }
	inline void SetIsActive(bool set) { mbIsActive = set; }
};