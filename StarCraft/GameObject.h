#pragma once

#include "Component.h"
#include "SpriteComponent.h"
#include "RendererComponent.h"
#include "UIRendererComponent.h"
#include "PairSpriteComponent.h"
#include "TransformComponent.h"
#include "EffectComponent.h"
#include "AnimatorComponent.h"
#include "PathFindComponent.h"
#include "ColliderComponent.h"
#include "StateMachineComponent.h"

class Scene;
class RenderManager;
class UIManager;
class GameObject
{
	friend Scene;
	friend RenderManager;
	friend UIManager;
private:
	vector<Component*> mVecComponent;
	vector<pair<int, GameObject*>> mVecChild;

	size_t mComponentSize = 0;
	size_t mChildSize = 0;

	void init();
	void release();
	void update();
	bool render(ID2D1DeviceContext2* pContext);
protected:
	TransformComponent* mpTransform = nullptr;
	RendererComponent* mpRenderer = nullptr;

public:
	GameObject();
	GameObject(RendererComponent* pRenderer);
	virtual ~GameObject();
	
	virtual void Init() {};
	virtual void Release() {};
	virtual void Update() {};

	Component* AddComponent(Component* pComponent);
	Component* GetComponent(eComponentTag tag);

	GameObject* AddChild(GameObject* pObject, int order);

	inline TransformComponent* GetTransform() { return mpTransform; }
	inline RendererComponent* GetRenderer() { return mpRenderer; }
};
