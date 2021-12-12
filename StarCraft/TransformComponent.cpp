#include "stdafx.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent()
{
	mOrder = -1;
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