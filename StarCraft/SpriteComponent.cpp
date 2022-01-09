#include "stdafx.h"
#include "SpriteComponent.h"
#include "GameObject.h"
#include "RendererComponent.h"
#include "RenderManager.h"

SpriteComponent::SpriteComponent(eBitmapTag tag, int frameX, int frameY, D2D1_POINT_2F anchor)
	:mMaxFrameX{ frameX }, mMaxFrameY{ frameY }, mAnchor{ anchor }
{
	mTag = eComponentTag::Sprite;
	mpBitmap = RENDER->GetBitmap(tag);
	mSize.width = mpBitmap->GetSize().width / frameX;
	mSize.height = mpBitmap->GetSize().height / frameY;
	SetCurFrame(0, 0);
}

SpriteComponent::~SpriteComponent()
{
	
}

void SpriteComponent::Init(GameObject* pObject)
{
	mpGameObject = pObject;
	mpGameObject->GetComponent(eComponentTag::Renderer)->LinkComponent(this);
}

void SpriteComponent::Release()
{
}

void SpriteComponent::Update()
{
}

void SpriteComponent::SetCurFrameX(int x)
{
	if (x < 0) { mCurFrameX = 0; }
	else if (x >= mMaxFrameX) { mCurFrameX = mMaxFrameX - 1; }
	else { mCurFrameX = x; }

	mOffset.x = mSize.width * mCurFrameX;
}

void SpriteComponent::SetCurFrameY(int y)
{
	if (y < 0) { mCurFrameY = 0; }
	else if (y >= mMaxFrameY) { mCurFrameY = mMaxFrameY - 1; }
	else { mCurFrameY = y; }

	mOffset.y = mSize.height * mCurFrameY;
}

void SpriteComponent::SetCurFrame(int x, int y)
{
	SetCurFrameX(x);
	SetCurFrameY(y);
}

void SpriteComponent::SetMaxFrame(int x, int y)
{
	mMaxFrameX = x;
	mMaxFrameY = y;
	mSize.width = mpBitmap->GetSize().width / mMaxFrameX;
	mSize.height = mpBitmap->GetSize().height / mMaxFrameY;
	SetCurFrame(0, 0);
	RendererComponent* pRenderer = static_cast<RendererComponent*>(mpGameObject->GetComponent(eComponentTag::Renderer));
	pRenderer->SetUnitLayer(pRenderer->GetUnitLayer());
}

void SpriteComponent::ChangeBitmap(eBitmapTag tag)
{
	mpBitmap = RENDER->GetBitmap(tag);
	mSize.width = mpBitmap->GetSize().width / mMaxFrameX;
	mSize.height = mpBitmap->GetSize().height / mMaxFrameY;
	SetCurFrame(0, 0);
}
