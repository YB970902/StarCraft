#pragma once

class GameObject;
class Component abstract
{
protected:
	GameObject* mpGameObject = nullptr;
	unsigned int mOrder = 0;

	eComponentTag mTag = eComponentTag::None;

public:
	Component() = default;
	virtual ~Component() = default;
	virtual void Init(GameObject* pObject) = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;

	virtual void LinkComponent(Component* pOther) { }

	inline unsigned int GetOrder() const { return mOrder; }

	inline eComponentTag GetTag() { return mTag; }

	GameObject* GetGameObject() { return mpGameObject; }
};