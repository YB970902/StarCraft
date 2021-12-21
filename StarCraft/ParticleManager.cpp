#include "stdafx.h"
#include "ParticleManager.h"
#include "Particle.h"
#include "Scene.h"

void ParticleManager::Init(Scene* pScene)
{
	mpCurScene = pScene;
}

void ParticleManager::Release()
{
}

void ParticleManager::Update()
{
	for (auto it = mListParticle.begin(); it != mListParticle.end();)
	{
		if ((*it)->IsEnd())
		{
			mpCurScene->RemoveGameObject(*it);
			it = mListParticle.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void ParticleManager::CreateParticle(eParticleTag tag, const Vector2& pos)
{
	Particle* newParticle = nullptr;

	switch (tag)
	{
	case eParticleTag::ParticleMarineDead:
		newParticle = new Particle(eUnitLayer::Ground, eBitmapTag::PARTICLE_MARINE_DEAD, ParticleMarineDead, eParticleTag::RemainsMarine);
		break;
	case eParticleTag::ParticleAttack:
		newParticle = new Particle(eUnitLayer::Particle, eBitmapTag::PARTICLE_ATTACK, ParticleAttack);
		break;
	case eParticleTag::ParticleExplosion:
		newParticle = new Particle(eUnitLayer::Particle, eBitmapTag::PARTICLE_EXPLOSION, ParticleExplosion);
		break;
	case eParticleTag::RemainsMarine:
		newParticle = new Particle(eUnitLayer::Remains, eBitmapTag::REMAINS_MARINE, RemainsMarine);
		break;
	default:
		return;
	}

	mListParticle.push_back(newParticle);
	mpCurScene->AddGameObject(newParticle);
	newParticle->GetTransform()->SetPosition(pos);
}
