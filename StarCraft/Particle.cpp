#include "stdafx.h"
#include "Particle.h"
#include "SingleAnimation.h"
#include "ParticleManager.h"

Particle::Particle(eParticleTag particleTag, eUnitLayer layer, eBitmapTag bitmapTag, const SingleAnimationClip& animClip, eParticleTag afterParticle)
	:mParticleTag{ particleTag }
{
	mpRenderer->SetUnitLayer(layer);
	mpSprite = static_cast<SpriteComponent*>(AddComponent(new SpriteComponent(bitmapTag, animClip.Length)));
	mpAnimator = static_cast<AnimatorComponent*>(AddComponent(new AnimatorComponent()));

	mpAnimator->AddSingleAnimation((int)eAnimationTag::Idle, new SingleAnimation(animClip));

	mAfterParticleTag = afterParticle;

	Init();
}

Particle::~Particle()
{
	Release();
}

void Particle::Init()
{
	mpAnimator->ChangeAnimation((int)eAnimationTag::Idle);
	mpAnimator->Play();
}

void Particle::Release()
{
	if (mAfterParticleTag != eParticleTag::None)
	{
		PARTICLE->CreateParticle(mAfterParticleTag, mpTransform->GetPosition());
	}
}

void Particle::Update()
{
}

bool Particle::IsEnd()
{
	return mpAnimator->IsEnd();
}
