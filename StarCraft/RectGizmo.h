#pragma once
#include "Gizmo.h"

class RectGizmo : public Gizmo
{
	Vector2 mSize;
	Vector2 mAnchor;
	float mWidth = 1;

public:
	RectGizmo(Vector2 pos, Vector2 size, Vector2 anchor, float width, ID2D1SolidColorBrush* pBrush);
	virtual ~RectGizmo();

	virtual void Render(ID2D1DeviceContext2* pD2DContext) override;
};

