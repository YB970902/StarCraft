#pragma once

#include "Component.h"
#include "SpriteComponent.h"
#include "RendererComponent.h"
#include "PairSpriteComponent.h"
#include "TransformComponent.h"
#include "EffectComponent.h"
#include "AnimatorComponent.h"
#include "PathFindComponent.h"
#include "ColliderComponent.h"

class Scene;
class GameObject
{
	friend Scene;
private:
	vector<Component*> mVecComponent;
	vector<GameObject*> mVecChild;

	size_t mComponentSize = 0;
	size_t mChildSize = 0;

	void init();
	void release();
	void update();
protected:
	TransformComponent* mpTransform = nullptr;
	RendererComponent* mpRenderer = nullptr;

public:
	GameObject();
	virtual ~GameObject();
	
	virtual void Init() {};
	virtual void Release() {};
	virtual void Update() {};

	Component* AddComponent(Component* pComponent);
	Component* GetComponent(eComponentTag tag);

	inline TransformComponent* GetTransform() { return mpTransform; }
	inline RendererComponent* GetRenderer() { return mpRenderer; }
};
