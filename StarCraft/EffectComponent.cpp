#include "stdafx.h"
#include "RenderConfig.h"
#include "EffectComponent.h"
#include "ColorReplaceEffect.h"
#include "RenderManager.h"
#include "GameObject.h"

EffectComponent::EffectComponent(eEffectTag tag)
{
	mpEffect = RENDER->CreateEffect(tag);
}

EffectComponent::~EffectComponent()
{
	
}

void EffectComponent::Init(GameObject* pObject)
{
	mpGameObject = pObject;
	mpGameObject->GetComponent(eComponentTag::Renderer)->LinkComponent(this);
}

void EffectComponent::Release()
{
	mpEffect->Release();
}

void EffectComponent::Update()
{
}
