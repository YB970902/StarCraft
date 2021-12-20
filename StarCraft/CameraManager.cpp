#include "stdafx.h"
#include "CameraManager.h"
#include "PhysicsManager.h"

void CameraManager::Init()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	ClipCursor(&rc);
	mSize.width = rc.right - rc.left;
	mSize.height = rc.bottom - rc.top;
	mCameraRect = RECT{ (long)-mPosition.x, (long)-mPosition.y, (long)-mPosition.x + (long)mSize.width, (long)-mPosition.y + (long)mSize.height };
}

void CameraManager::Update()
{
	if (mbIsMoved)
	{
		mCameraRect = RECT{ (long)-mPosition.x, (long)-mPosition.y, (long)-mPosition.x + (long)mSize.width, (long)-mPosition.y + (long)mSize.height };
		mbIsMoved = false;
	}
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

void CameraManager::SetPosition(Vector2 pos)
{
	if (pos.x > (Fix)0) { pos.x = 0; }
	else if (pos.x < -(Fix)(mMaxMovingSize.x - mSize.width)) { pos.x = -(mMaxMovingSize.x - mSize.width); }

	if (pos.y > (Fix)0) { pos.y = 0; }
	else if (pos.y < -(Fix)(mMaxMovingSize.y - mSize.height)) { pos.y = -(mMaxMovingSize.y - mSize.height); }

	mPosition = pos;
	mbIsMoved = true;
}

void CameraManager::AddPosition(Vector2 add)
{
	mPosition += add;

	if (mPosition.x > (Fix)0) { mPosition.x = 0; }
	else if (mPosition.x < -(Fix)(mMaxMovingSize.x - mSize.width)) { mPosition.x = -(mMaxMovingSize.x - mSize.width); }

	if (mPosition.y > (Fix)0) { mPosition.y = 0; }
	else if (mPosition.y < -(Fix)(mMaxMovingSize.y - mSize.height)) { mPosition.y = -(mMaxMovingSize.y - mSize.height); }

	mbIsMoved = true;
}
