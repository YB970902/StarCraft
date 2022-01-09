#include "stdafx.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent()
{
	mOrder = 20;
	mTag = eComponentTag::Transform;
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::Init(GameObject* pObject)
{

}

void TransformComponent::Release()
{

}

void TransformComponent::Update()
{
	mbIsMoved = false;
}

void TransformComponent::SetPosition(const Fix& x, const Fix& y)
{
	SetPosition(Vector2(x, y));
}

void TransformComponent::SetPosition(const POINT& pos)
{
	SetPosition(Vector2(pos.x, pos.y));
}

void TransformComponent::SetPosition(const Vector2& pos)
{
	mbIsMoved = true;
	if (mpParent)
	{
		mLocalPosition = pos - mpParent->GetPosition();
	}
	else
	{
		mLocalPosition = pos;
	}
}

void TransformComponent::AddPosition(const Vector2& add)
{
	mbIsMoved = true;
	mLocalPosition += add;
}

Vector2 TransformComponent::GetPosition()
{
	if (mpParent)
	{
		return (mpParent->GetPosition() + mLocalPosition);
	}
	else
	{
		return mLocalPosition;
	}
}

Fix TransformComponent::GetRotation()
{
	if (mpParent)
	{
		return (mpParent->GetRotation() + mLocalRotation);
	}
	else
	{
		return mLocalRotation;
	}
}

void TransformComponent::SetRotation(const Fix& rot)
{
	if (mpParent)
	{
		mLocalRotation = rot - mpParent->GetRotation();
	}
	else
	{
		mLocalRotation = rot;
	}
}
