#include "stdafx.h"
#include "ColliderComponent.h"
#include "GameObject.h"
#include "PhysicsManager.h"
#include "RendererComponent.h"

ColliderComponent::ColliderComponent(int size)
	: mSize{ size }
{
	mOrder = 1;
}

void ColliderComponent::Init(GameObject* pObject)
{
	mpGameObject = pObject;
	mpTransform = mpGameObject->GetTransform();
	PHYSICS->ProcessObjectMove(mpGameObject, mpGameObject->GetRenderer()->GetRectPosition(), mLeftTop, mRightBottom, true);
}

void ColliderComponent::Release()
{
}

void ColliderComponent::Update()
{
	if (mpTransform->IsMoved())
	{
		PHYSICS->ProcessObjectMove(mpGameObject, mpGameObject->GetRenderer()->GetRectPosition(), mLeftTop, mRightBottom);
	}
}