#pragma once
#include "Component.h"

class TransformComponent;
class ColliderComponent : public Component
{
private:
	POINT mLeftTop = { -1, -1 };
	POINT mRightBottom = { -1, -1 };

	int mSize = 0;

	TransformComponent* mpTransform = nullptr;

public:
	ColliderComponent(int size);
	virtual ~ColliderComponent() = default;
	virtual void Init(GameObject* pObject) override;
	virtual void Release() override;
	virtual void Update() override;
	inline virtual eComponentTag GetTag() override { return eComponentTag::Collider; }
};