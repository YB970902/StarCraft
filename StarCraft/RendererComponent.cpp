#include "stdafx.h"
#include "RendererComponent.h"
#include "RenderManager.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "EffectComponent.h"

RendererComponent::RendererComponent()
{
	mOrder = 10;
}

RendererComponent::~RendererComponent()
{
}

void RendererComponent::Init(GameObject* pObject)
{
	mpGameObject = pObject;
	mpTransform = (TransformComponent*)mpGameObject->GetComponent(eComponentTag::Transform);
	mPrevPosition = mpTransform->GetPosition();
	RENDER->AddRenderer(mPrevPosition, this);
}

void RendererComponent::Release()
{
	RENDER->EraseRenderer(mPrevPosition, this);
}

void RendererComponent::Update()
{
	if (mpTransform->IsMoved())
	{
		RENDER->RendererMoved(this, mPrevPosition, mpTransform->GetPosition());
		mPrevPosition = mpTransform->GetPosition();

		mRect = D2D_RECT_F{
		(float)mpTransform->GetPosition().x + mpSprite->GetAnchorX(),
		(float)mpTransform->GetPosition().y + mpSprite->GetAnchorY(),
		(float)mpTransform->GetPosition().x + mpSprite->GetAnchorX() + mpSprite->GetSizeWidth(),
		(float)mpTransform->GetPosition().y + mpSprite->GetAnchorY() + mpSprite->GetSizeHeight()
		};
	}
}

void RendererComponent::Render(ID2D1DeviceContext2* pContext)
{
	if (mbIsRender)
	{
		if (mpEffect)
		{
			pContext->DrawImage(mpEffect->GetEffect(), GetPosition(), mpSprite->GetSourceRect());
		}
		else if (mpSprite)
		{
			pContext->DrawBitmap(mpSprite->GetBitmap(), GetRectPosition(), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, mpSprite->GetSourceRect());
		}
	}
}

D2D_POINT_2F RendererComponent::GetPosition()
{
	return D2D_POINT_2F{
		(float)mpTransform->GetPosition().x + mpSprite->GetAnchorX(),
		(float)mpTransform->GetPosition().y + mpSprite->GetAnchorY()
	};
}

void RendererComponent::LinkComponent(Component* pOther)
{
	switch (pOther->GetTag())
	{
	case eComponentTag::Sprite:
	case eComponentTag::PairSprite:
		mpSprite = (SpriteComponent*)pOther;
		break;
	case eComponentTag::Effect:
		mpEffect = (EffectComponent*)pOther;
		break;
	}
}

void RendererComponent::SetUnitLayer(eUnitLayer layer)
{
	RENDER->EraseRenderer(mPrevPosition, this);
	mUnitLayer = layer;
	mPrevPosition = mpTransform->GetPosition();
	RENDER->AddRenderer(mPrevPosition, this);
}

int RendererComponent::GetUnitY() const
{
	return mpTransform->GetPosition().y;
}
