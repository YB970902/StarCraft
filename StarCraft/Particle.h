#pragma once
#include "GameObject.h"

class Particle : public GameObject
{
protected:
	SpriteComponent* mpSprite = nullptr;
	AnimatorComponent* mpAnimator = nullptr;

	eParticleTag mAfterParticleTag;
public:
	Particle(eUnitLayer layer, eBitmapTag bitmapTag, const SingleAnimationClip& animClip, eParticleTag afterParticle = eParticleTag::None);
	virtual ~Particle();

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;

	bool IsEnd();
};

