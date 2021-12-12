#include "stdafx.h"
#include "CameraManager.h"

void CameraManager::Init()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	mSize.width = rc.right - rc.left;
	mSize.height = rc.bottom - rc.top;
}

bool CameraManager::IsInCamera(D2D_RECT_F rect)
{
	if (rect.left < -(float)mPosition.x + mSize.width && rect.right > -(float)mPosition.x &&
		rect.top < -(float)mPosition.y + mSize.height && rect.bottom > -(float)mPosition.y)
	{
		return true;
	}
	return false;
}
