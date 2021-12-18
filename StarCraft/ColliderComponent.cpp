#include "stdafx.h"
#include "ColliderComponent.h"
#include "PhysicsManager.h"
#include "GameObject.h"
#include "Unit.h"
#include "TransformComponent.h"

ColliderComponent::ColliderComponent(const Vector2& localPos, const Vector2& size, eTeamTag teamTag)
	: mLocalPos{ localPos }, mSize{ size }, mTeamTag{ teamTag }
{
	mOrder = 1;
}

void ColliderComponent::Init(GameObject* pObject)
{
	mpGameObject = pObject;
	mpUnit = static_cast<Unit*>(mpGameObject);

	mTeamTag = mpUnit->GetTeamTag();
	mpTransform = mpGameObject->GetTransform();
	mPrevPosition = mpUnit->GetColliderPosition();

	PHYSICS->AddCollider(this, mPrevPosition, mSize);
}

void ColliderComponent::Release()
{
	PHYSICS->RemoveCollider(this, mPrevPosition, mSize);
}

void ColliderComponent::Update()
{
	if (mpTransform->IsMoved())
	{
		PHYSICS->ProcessObjectMove(this, mPrevPosition, mSize);
		mPrevPosition = mpUnit->GetColliderPosition();
	}
}

Vector2 ColliderComponent::GetColliderPosition()
{
	if (mpTransform)
	{
		return mpTransform->GetPosition() + mLocalPos;
	}
	return mLocalPos;
}

bool ColliderComponent::IsCollided(const POINT& point)
{
	Vector2 pos = GetColliderPosition();
	Vector2 halfSize = mSize * 0.5f;
	RECT rc = { (long)(pos.x - halfSize.x), (long)(pos.y - halfSize.y), (long)(pos.x + halfSize.x), (long)(pos.y + halfSize.y) };
	
	if (point.x >= rc.left &&
		point.x <= rc.right &&
		point.y >= rc.top &&
		point.y <= rc.bottom)
	{
		return true;
	}
	return false;
}
