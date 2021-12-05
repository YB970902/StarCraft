#pragma once
#include "Gizmo.h"

class RectGizmo : public Gizmo
{
	Vector2 mSize;
	Vector2 mAnchor;

public:
	RectGizmo(Vector2 pos, Vector2 size, Vector2 anchor, ID2D1SolidColorBrush* pBrush);
	virtual ~RectGizmo();

	virtual void Render(ID2D1DeviceContext2* pD2DContext) override;
};

