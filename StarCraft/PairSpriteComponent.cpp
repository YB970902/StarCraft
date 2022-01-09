#include "stdafx.h"
#include "PairSpriteComponent.h"
#include "RenderManager.h"
#include "GameObject.h"

PairSpriteComponent::PairSpriteComponent(eBitmapTag leftTag, eBitmapTag rightTag, int frameX, int frameY, D2D1_POINT_2F anchor)
	:SpriteComponent(leftTag, frameX, frameY, anchor)
{
	mTag = eComponentTag::PairSprite;
	mpLeftBitmap = mpBitmap;
	mpRightBitmap = RENDER->GetBitmap(rightTag);
}

PairSpriteComponent::~PairSpriteComponent()
{
	mpLeftBitmap = nullptr;
	mpRightBitmap = nullptr;
}

void PairSpriteComponent::Init(GameObject* pObject)
{
	mpGameObject = pObject;
	mpGameObject->GetComponent(eComponentTag::Renderer)->LinkComponent(this);
	Component* pAnimator = mpGameObject->GetComponent(eComponentTag::Animator);
	if (pAnimator) { pAnimator->LinkComponent(this); }
}

void PairSpriteComponent::Release()
{
}

void PairSpriteComponent::Update()
{
}
