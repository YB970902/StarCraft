#pragma once
#include "Gizmo.h"

class LineGizmo : public Gizmo
{
private:
	Vector2 mEndPos;
	int mWidth;

public:
	LineGizmo(Vector2 startPos, Vector2 endPos, int width, ID2D1SolidColorBrush* pBrush);

	virtual void Render(ID2D1DeviceContext2* pD2DContext);

	inline void SetStartPosition(const Vector2& startPos) { mPosition.x = startPos.x; mPosition.y = startPos.y; }
	inline void SetEndPosition(const Vector2& endPos) { mEndPos.x = endPos.x;  mEndPos.y = endPos.y; }
};