#include "stdafx.h"
#include "RectGizmo.h"

RectGizmo::RectGizmo(Vector2 pos, Vector2 size, Vector2 anchor, ID2D1SolidColorBrush* pBrush)
	:Gizmo(pos, pBrush), mSize{ size }, mAnchor{ anchor }
{

}

RectGizmo::~RectGizmo()
{
	mpBrush->Release();
}

void RectGizmo::Render(ID2D1DeviceContext2* pD2DContext)
{
	pD2DContext->DrawRectangle(D2D1_RECT_F{
		mPosition.x - mSize.x * mAnchor.x,
		mPosition.y - mSize.y * mAnchor.y,
		mPosition.x + mSize.x - mSize.x * mAnchor.x,
		mPosition.y + mSize.y - mSize.y * mAnchor.y
		}, mpBrush);
}