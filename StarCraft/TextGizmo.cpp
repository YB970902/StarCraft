#include "stdafx.h"
#include "TextGizmo.h"

TextGizmo::TextGizmo(IDWriteTextFormat* pFormat, wstring text, Vector2 pos, Vector2 size, ID2D1SolidColorBrush* pBrush)
	:Gizmo(pos, pBrush), mSize{ size }, mpFormat{ pFormat }, mText{ text }
{

}

TextGizmo::~TextGizmo()
{
	mpBrush->Release();
	mpFormat->Release();
}

void TextGizmo::Render(ID2D1DeviceContext2* pD2DContext)
{
	pD2DContext->DrawTextW(mText.c_str(), mText.length(), mpFormat, D2D1_RECT_F{ mPosition.x, mPosition.y, mPosition.x + mSize.x, mPosition.y + mSize.y }, mpBrush);
}
