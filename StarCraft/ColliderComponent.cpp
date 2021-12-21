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
	mPrevPosition = mpUnit->GetPosition();

	PHYSICS->AddCollider(this, mPrevPosition, mSize);
}

void ColliderComponent::Release()
{
	PHYSICS->RemoveCollider(this, mPrevPosition, mSize);
	PHYSICS->RemoveCollider(this, mpUnit->GetPosition(), mSize);
}

void ColliderComponent::Update()
{
	if (mpTransform->IsMoved())
	{
		PHYSICS->ProcessObjectMove(this, mPrevPosition, mSize);
		mPrevPosition = mpUnit->GetPosition();
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
	RECT rc = { (long)(pos.x - mSize.x), (long)(pos.y - mSize.y), (long)(pos.x + mSize.x), (long)(pos.y + mSize.y) };
	
	if (point.x >= rc.left &&
		point.x <= rc.right &&
		point.y >= rc.top &&
		point.y <= rc.bottom)
	{
		return true;
	}
	return false;
}

bool ColliderComponent::IsCollided(const RECT& rc)
{
	if (mpUnit->IsDead()) { return false; }
	Vector2 pos = GetColliderPosition();
	Vector2 halfSize = mSize * 0.5f;
	RECT myRc = { (long)(pos.x - halfSize.x), (long)(pos.y - halfSize.y), (long)(pos.x + halfSize.x), (long)(pos.y + halfSize.y) };

	if (rc.right >= myRc.left &&
		rc.left <= myRc.right &&
		rc.bottom >= myRc.top &&
		rc.top <= myRc.bottom)
	{
		return true;
	}
	return false;
}
