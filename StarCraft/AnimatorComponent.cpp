#include "stdafx.h"
#include "AnimatorComponent.h"
#include "GameObject.h"
#include "AnimationBase.h"
#include "SpriteComponent.h"
#include "PairSpriteComponent.h"
#include "SingleAnimation.h"
#include "MultipleAnimation.h"
#include "EffectComponent.h"

void AnimatorComponent::Init(GameObject* pObject)
{
	mTag = eComponentTag::Animator;
	mpGameObject = pObject;
	mpSprite = static_cast<SpriteComponent*>(mpGameObject->GetComponent(eComponentTag::Sprite));
	mpPairSprite = static_cast<PairSpriteComponent*>(mpGameObject->GetComponent(eComponentTag::PairSprite));
	mpEffect = static_cast<EffectComponent*>(mpGameObject->GetComponent(eComponentTag::Effect));
	mpTrasnform = static_cast<TransformComponent*>(mpGameObject->GetComponent(eComponentTag::Transform));
}

void AnimatorComponent::Release()
{
	for (auto it = mMapAnimation.begin(); it != mMapAnimation.end(); )
	{
		delete it->second;
		it = mMapAnimation.erase(it);
	}
}

void AnimatorComponent::Update()
{
	if (mpCurAnimation) {

		mpCurAnimation->Update();

		if (mpTrasnform)
		{
			mpCurAnimation->SetAngle(mpTrasnform->GetRotation());
		}

		if (mpSprite)
		{
			mpSprite->SetCurFrame(mpCurAnimation->GetCol(), mpCurAnimation->GetRow());
		}
		else if (mpPairSprite)
		{
			mpPairSprite->SetCurFrame(mpCurAnimation->GetCol(), mpCurAnimation->GetRow());
			mpPairSprite->SetBitmap(mpCurAnimation->GetBitmap());
			if (mpEffect)
			{
				mpEffect->GetEffect()->SetInput(0, mpCurAnimation->GetBitmap());
			}
		}
	}
}

void AnimatorComponent::LinkComponent(Component* pComponent)
{
	switch (pComponent->GetTag())
	{
	case eComponentTag::Sprite:
		mpSprite = (SpriteComponent*)pComponent;
		break;
	case eComponentTag::PairSprite:
		mpPairSprite = (PairSpriteComponent*)pComponent;
		break;
	case eComponentTag::Effect:
		mpEffect = (EffectComponent*)pComponent;
		break;
	}
}

void AnimatorComponent::AddSingleAnimation(int animTag, SingleAnimation* pAnimation)
{
	mMapAnimation[animTag] = (AnimationBase*)pAnimation;
	pAnimation->Init(mpSprite->GetBitmap());
}

void AnimatorComponent::AddMultipleAnimation(int animTag, MultipleAnimation* pAnimation)
{
	mMapAnimation[animTag] = (AnimationBase*)pAnimation;
	pAnimation->Init(mpPairSprite->GetLeftBitmap(), mpPairSprite->GetRightBitmap());
}

void AnimatorComponent::ChangeAnimation(int animTag)
{
	if (mpCurAnimation)
	{
		if (mpCurAnimation == mMapAnimation[animTag]) { return; }
		mpCurAnimation->Stop();
	}
	mpCurAnimation = mMapAnimation[animTag];
	mpCurAnimation->Play();
}

void AnimatorComponent::Play()
{
	if (IsEnd()) { mpCurAnimation->Play(); }
}

void AnimatorComponent::Stop()
{
	mpCurAnimation->Stop();
}

bool AnimatorComponent::IsEnd()
{
	if (mpCurAnimation) { return mpCurAnimation->IsEnd(); }
	return true;
}
