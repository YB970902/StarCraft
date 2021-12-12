#pragma once
#include "Component.h"

class TransformComponent : public Component
{
private:
	Vector2 mPosition = { 0, 0 };
	Fix mRotation = 0;
	bool mbIsMoved = false;
public:
	TransformComponent();
	virtual ~TransformComponent();

	virtual void Init(GameObject* pObject) override;
	virtual void Release() override;
	virtual void Update() override;

	inline void SetPosition(const Fix& x, const Fix& y) { mPosition.x = x; mPosition.y = y; mbIsMoved = true; }
	inline void SetPosition(const POINT& pos) { mPosition.x = pos.x, mPosition.y = pos.y; mbIsMoved = true; }
	inline void SetPosition(const Vector2& pos) { mPosition.x = pos.x; mPosition.y = pos.y; mbIsMoved = true; }
	inline void AddPosition(const Vector2& add) { mPosition += add; mbIsMoved = true; }
	inline Vector2 GetPosition() { return mPosition; }
	inline D2D_POINT_2F GetPointPosition() { return D2D_POINT_2F{ mPosition.x, mPosition.y }; }
	inline Fix GetRotation() { return mRotation; }
	inline void SetRotation(const Fix& rot) { mRotation = rot; }

	inline bool IsMoved() { return mbIsMoved; }

	virtual eComponentTag GetTag() override { return eComponentTag::Transform; }
};

