#include "stdafx.h"
#include "RendererComponent.h"
#include "RenderManager.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "EffectComponent.h"

void RendererComponent::Init(GameObject* pObject)
{
	mpGameObject = pObject;
	mpTransform = (TransformComponent*)mpGameObject->GetComponent(eComponentTag::Transform);
}

void RendererComponent::Release()
{
}

void RendererComponent::Update()
{
	// 렌더가 가능한 순간이라면
	RENDER->PushRenderer(mpTransform->GetPosition().y, this);
}

void RendererComponent::Render(ID2D1DeviceContext2* pContext)
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

D2D_RECT_F RendererComponent::GetRectPosition()
{
	return D2D_RECT_F{
		(float)mpTransform->GetPosition().x + mpSprite->GetAnchorX(),
		(float)mpTransform->GetPosition().y + mpSprite->GetAnchorY(),
		(float)mpTransform->GetPosition().x + mpSprite->GetAnchorX() + mpSprite->GetSizeWidth(),
		(float)mpTransform->GetPosition().y + mpSprite->GetAnchorY() + mpSprite->GetSizeHeight()
	};
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
