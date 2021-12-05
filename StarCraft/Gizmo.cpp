#include "stdafx.h"
#include "Gizmo.h"

Gizmo::Gizmo(Vector2 position, ID2D1SolidColorBrush* pBrush)
	:mPosition{ position }, mpBrush{ pBrush }
{
}
