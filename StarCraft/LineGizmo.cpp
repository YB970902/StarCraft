#include "stdafx.h"
#include "LineGizmo.h"

LineGizmo::LineGizmo(Vector2 startPos, Vector2 endPos, int width, ID2D1SolidColorBrush* pBrush)
	: Gizmo(startPos, pBrush), mEndPos{ endPos }, mWidth{ width }
{

}

void LineGizmo::Render(ID2D1DeviceContext2* pD2DContext)
{
	pD2DContext->DrawLine(D2D1::Point2F(mPosition.x, mPosition.y), D2D1::Point2F(mEndPos.x, mEndPos.y), mpBrush, mWidth);
}
