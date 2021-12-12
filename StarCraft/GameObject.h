#pragma once

#include "Component.h"
#include "SpriteComponent.h"
#include "PairSpriteComponent.h"
#include "TransformComponent.h"
#include "EffectComponent.h"
#include "AnimatorComponent.h"
#include "PathFindComponent.h"

class Scene;
class GameObject
{
	friend Scene;
private:
	vector<Component*> mVecComponent;
	vector<GameObject*> mVecChild;

	void init();
	void release();
	void update();
protected:
	virtual void Init() {};
	virtual void Release() {};
	virtual void Update() {};
public:

	GameObject();
	virtual ~GameObject();

	Component* AddComponent(Component* pComponent);
	Component* GetComponent(eComponentTag tag);
};
