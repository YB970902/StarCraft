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
	for (auto it = mListParticle.begin(); it != mListParticle.end();)
	{
		mpCurScene->RemoveGameObject(*it);
		it = mListParticle.erase(it);
	}

	for (auto it = mListOnceParticle.begin(); it != mListOnceParticle.end();)
	{
		mpCurScene->RemoveGameObject(*it);
		it = mListOnceParticle.erase(it);
	}
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

	for (auto it = mListOnceParticle.begin(); it != mListOnceParticle.end();)
	{
		if ((*it)->IsEnd())
		{
			mpCurScene->RemoveGameObject(*it);
			it = mListOnceParticle.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void ParticleManager::CreateParticle(eParticleTag tag, const Vector2& pos)
{
	Particle* newParticle = InstantiateParticle(tag);

	if (newParticle == nullptr) { return; }

	mListParticle.push_back(newParticle);
	mpCurScene->AddGameObject(newParticle);
	newParticle->GetTransform()->SetPosition(pos);
}

void ParticleManager::CreateOnceParticle(eParticleTag tag, const Vector2& pos)
{
	for (auto it = mListOnceParticle.begin(); it != mListOnceParticle.end();)
	{
		if ((*it)->GetParticleTag() == tag)
		{
			mpCurScene->RemoveGameObject(*it);
			mListOnceParticle.erase(it);
			break;
		}
		else
		{
			++it;
		}
	}

	Particle* newParticle = InstantiateParticle(tag);

	if (newParticle == nullptr) { return; }

	mListOnceParticle.push_back(newParticle);
	mpCurScene->AddGameObject(newParticle);
	newParticle->GetTransform()->SetPosition(pos);
}

Particle* ParticleManager::InstantiateParticle(eParticleTag tag)
{
	switch (tag)
	{
	case eParticleTag::ClickCircle:
		return new Particle(tag, eUnitLayer::Ground, eBitmapTag::UI_CLICK_CIRCLE, ClickCircle);
	case eParticleTag::ParticleMarineDead:
		return new Particle(tag, eUnitLayer::Ground, eBitmapTag::PARTICLE_MARINE_DEAD, ParticleMarineDead, eParticleTag::RemainsMarine);
	case eParticleTag::ParticleAttack:
		return new Particle(tag, eUnitLayer::Particle, eBitmapTag::PARTICLE_ATTACK, ParticleAttack);
	case eParticleTag::ParticleExplosion:
		return new Particle(tag, eUnitLayer::Particle, eBitmapTag::PARTICLE_EXPLOSION, ParticleExplosion);
	case eParticleTag::RemainsMarine:
		return new Particle(tag, eUnitLayer::Remains, eBitmapTag::REMAINS_MARINE, RemainsMarine);
	default:
		return nullptr;
	}
}
