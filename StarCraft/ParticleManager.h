#pragma once
#include "Singleton.h"

#define PARTICLE ParticleManager::GetInstance()

class Scene;
class Particle;
class ParticleManager : public Singleton<ParticleManager>
{
private:
	Scene* mpCurScene = nullptr;
	list<Particle*> mListParticle;

public:
	void Init(Scene* pScene);
	void Release();
	void Update();

	void CreateParticle(eParticleTag tag, const Vector2& pos);
};

