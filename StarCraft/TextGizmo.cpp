#include "stdafx.h"
#include "TextGizmo.h"

TextGizmo::TextGizmo(IDWriteTextFormat* pFormat, wstring text, Vector2 pos, Vector2 size, ID2D1SolidColorBrush* pBrush)
	:Gizmo(pBrush), mSize{ size }, mpFormat{ pFormat }, mText{ text }
{
	SetPosition(pos);
}

TextGizmo::~TextGizmo()
{
	mpBrush->Release();
	mpFormat->Release();
}

void TextGizmo::Render(ID2D1DeviceContext2* pD2DContext)
{
	pD2DContext->DrawTextW(mText.c_str(), mText.length(), mpFormat, D2D1_RECT_F{
		mpTransform->GetPosition().x,
		mpTransform->GetPosition().y,
		mpTransform->GetPosition().x + mSize.x,
		mpTransform->GetPosition().y + mSize.y
		}, mpBrush
	);
}
