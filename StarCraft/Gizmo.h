#pragma once
#include "UIGameObject.h"

class Gizmo : public UIGameObject
{
protected:
	ID2D1SolidColorBrush* mpBrush = nullptr;
public:
	Gizmo(ID2D1SolidColorBrush* pBrush);
	virtual ~Gizmo() = default;

	virtual void Render(ID2D1DeviceContext2* pD2DContext) { };

	inline void SetPosition(Vector2 set) { mpTransform->SetPosition(set); }
	inline Vector2 GetPosition() { return mpTransform->GetPosition(); }
	inline void SetColor(D2D1::ColorF color) { if (mpBrush) { mpBrush->SetColor(color); } }

	inline bool IsRender() { return mpRenderer->IsRender(); }
	inline void SetIsRender(bool set) { mpRenderer->SetIsRender(set); }
};