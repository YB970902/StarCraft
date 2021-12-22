#include "stdafx.h"
#include "RectGizmo.h"

RectGizmo::RectGizmo(Vector2 pos, Vector2 size, Vector2 anchor, float width, ID2D1SolidColorBrush* pBrush)
	:Gizmo(pBrush), mSize{ size }, mAnchor{ anchor }, mWidth{ width }
{
	SetPosition(pos);
}

RectGizmo::~RectGizmo()
{
	mpBrush->Release();
}

void RectGizmo::Render(ID2D1DeviceContext2* pD2DContext)
{
	if (mpRenderer->IsRender())
	{
		pD2DContext->DrawRectangle(D2D1_RECT_F{
			mpTransform->GetPosition().x - mSize.x * mAnchor.x,
			mpTransform->GetPosition().y - mSize.y * mAnchor.y,
			mpTransform->GetPosition().x + mSize.x - mSize.x * mAnchor.x,
			mpTransform->GetPosition().y + mSize.y - mSize.y * mAnchor.y
			}, mpBrush, mWidth);
	}
}