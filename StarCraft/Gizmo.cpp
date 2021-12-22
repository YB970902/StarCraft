#include "stdafx.h"
#include "Gizmo.h"

Gizmo::Gizmo(ID2D1SolidColorBrush* pBrush)
	: UIGameObject::UIGameObject(), mpBrush{ pBrush }
{
}
