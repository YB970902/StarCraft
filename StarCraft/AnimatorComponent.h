#pragma once
#include "Component.h"
#include "AnimatorData.h"

class AnimationBase;
class SingleAnimation;
class MultipleAnimation;
class SpriteComponent;
class PairSpriteComponent;
class TransformComponent;
class EffectComponent;
class AnimatorComponent : public Component
{
private:
	SpriteComponent* mpSprite = nullptr;
	PairSpriteComponent* mpPairSprite = nullptr;
	EffectComponent* mpEffect = nullptr;
	TransformComponent* mpTrasnform = nullptr;
	
	unordered_map<int, AnimationBase*> mMapAnimation;
	
	AnimationBase* mpCurAnimation = nullptr;
public:
	AnimatorComponent() = default;
	virtual ~AnimatorComponent() {}
	virtual void Init(GameObject* pObject) override;
	virtual void Release() override;
	virtual void Update() override;

	virtual void LinkComponent(Component* pComponent);

	void AddSingleAnimation(int animTag, SingleAnimation* pAnimation);
	void AddMultipleAnimation(int animTag, MultipleAnimation* pAnimation);

	void ChangeAnimation(int animTag);
	void Play();
	void Stop();
	bool IsEnd();

	virtual eComponentTag GetTag() override { return eComponentTag::Animator; }
};