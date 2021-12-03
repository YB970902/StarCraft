#pragma once

class GameObject;
class Component abstract
{
protected:
	GameObject* mpGameObject = nullptr;

public:
	Component() = default;
	virtual ~Component() = default;
	virtual void Init(GameObject* pObject) = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;

	virtual void LinkComponent(Component* pOther) { }

	virtual eComponentTag GetTag() = 0;
};