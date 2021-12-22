#include "stdafx.h"
#include "UIRendererComponent.h"
#include "GameObject.h"
#include "RenderManager.h"

UIRendererComponent::UIRendererComponent()
{
	mOrder = 10;
	mbIsRender = true;
}

UIRendererComponent::~UIRendererComponent()
{
}

void UIRendererComponent::Init(GameObject* pObject)
{
	mpGameObject = pObject;
	mpTransform = (TransformComponent*)mpGameObject->GetComponent(eComponentTag::Transform);
	mPrevPosition = mpTransform->GetPosition();
}

void UIRendererComponent::Release()
{
	
}

void UIRendererComponent::Update()
{
	mRect = D2D_RECT_F{
		(float)mpTransform->GetPosition().x + mpSprite->GetAnchorX(),
		(float)mpTransform->GetPosition().y + mpSprite->GetAnchorY(),
		(float)mpTransform->GetPosition().x + mpSprite->GetAnchorX() + mpSprite->GetSizeWidth(),
		(float)mpTransform->GetPosition().y + mpSprite->GetAnchorY() + mpSprite->GetSizeHeight()
	};
}
