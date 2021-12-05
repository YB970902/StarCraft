#pragma once
#include "RenderConfig.h"

class Gizmo
{
protected:
	Vector2 mPosition;
	ID2D1SolidColorBrush* mpBrush = nullptr;
public:
	Gizmo(Vector2 position, ID2D1SolidColorBrush* pBrush);
	virtual ~Gizmo() = default;
	virtual void Render(ID2D1DeviceContext2* pD2DContext) { };

	virtual void SetPosition(Vector2 set) { mPosition = set; }
	virtual Vector2 GetPosition() { return mPosition; }
	virtual void SetColor(D2D1::ColorF color) { if (mpBrush) { mpBrush->SetColor(color); } }
};