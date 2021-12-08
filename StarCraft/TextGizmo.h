#pragma once
#include "Gizmo.h"

class TextGizmo : public Gizmo
{
	IDWriteTextFormat* mpFormat = nullptr;
	wstring mText;
	Vector2 mSize;
public:
	TextGizmo(IDWriteTextFormat* pFormat, wstring text, Vector2 pos, Vector2 size, ID2D1SolidColorBrush* pBrush);
	virtual ~TextGizmo();
	virtual void Render(ID2D1DeviceContext2* pD2DContext) override;

	void SetText(wstring text) { mText = text; }
};