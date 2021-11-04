#include "stdafx.h"
#include "GameObject.h"

void GameObject::Init()
{
	mPosition.x = 0;
	mPosition.y = 0;

	mTargetPosition.x = 0;
	mTargetPosition.y = 0;

	mbIsMoving = false;

	mAngle = 0.0f;
}

void GameObject::Release()
{
}

void GameObject::Update()
{
	if (mbIsMoving)
	{
		mPosition.x += cosf(mAngle);
		mPosition.y += sinf(mAngle);

		if ((mPosition.x - mTargetPosition.x) * (mPosition.x - mTargetPosition.x) +
			(mPosition.y - mTargetPosition.y) * (mPosition.y - mTargetPosition.y) < 0.1f)
		{
			mPosition = mTargetPosition;
			mbIsMoving = false;
		}
	}
}

void GameObject::Render(HDC hdc)
{
	Ellipse(hdc, mPosition.x - 10, mPosition.y - 10, mPosition.x + 10, mPosition.y + 10);
}

void GameObject::MoveTo(const POINTFLOAT& target)
{
	mbIsMoving = true;
	mTargetPosition = target;
	mAngle = atan2(target.y - mPosition.y, target.x - mPosition.x);
}
