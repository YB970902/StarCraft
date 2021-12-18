#pragma once
#include "Component.h"

class Unit;
class TransformComponent;
class ColliderComponent : public Component
{
private:
	Vector2 mSize;
	Vector2 mLocalPos;
	Vector2 mPrevPosition;

	eTeamTag mTeamTag = eTeamTag::RED_TEAM;

	Unit* mpUnit = nullptr;
	TransformComponent* mpTransform = nullptr;

public:
	ColliderComponent(const Vector2& localPos, const Vector2& size, eTeamTag teamTag);
	virtual ~ColliderComponent() = default;
	virtual void Init(GameObject* pObject) override;
	virtual void Release() override;
	virtual void Update() override;
	inline virtual eComponentTag GetTag() override { return eComponentTag::Collider; }

	Vector2 GetColliderPosition();
	inline Vector2 GetColliderSize() { return mSize; }

	inline Unit* GetUnit() { return mpUnit; }
	inline eTeamTag GetTeamTag() { return mTeamTag; }

	bool IsCollided(const POINT& point);
};