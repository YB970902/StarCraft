#include "stdafx.h"
#include "UIRendererComponent.h"
#include "GameObject.h"
#include "RenderManager.h"

UIRendererComponent::UIRendererComponent()
{
	mOrder = 10;
}

UIRendererComponent::~UIRendererComponent()
{
}

void UIRendererComponent::Init(GameObject* pObject)
{
	mpGameObject = pObject;
	mpTransform = (TransformComponent*)mpGameObject->GetComponent(eComponentTag::Transform);
	mPrevPosition = mpTransform->GetPosition();
	RENDER->AddUIRenderer(this);
}