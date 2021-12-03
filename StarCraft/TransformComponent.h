#pragma once
#include "Component.h"
#include "RenderConfig.h"

class TransformComponent : public Component
{
private:
	Vector2 mPosition = { 0, 0 };
	Fix mRotation = 0;
public:
	virtual void Init(GameObject* pObject) override;
	virtual void Release() override;
	virtual void Update() override;

	inline void SetPosition(Fix x, Fix y) { mPosition.x = x; mPosition.y = y; }
	inline void SetPosition(POINT pos) { mPosition.x = pos.x, mPosition.y = pos.y; }
	inline void SetPosition(Vector2 pos) { mPosition = pos; }
	inline Vector2 GetPosition() { return mPosition; }
	inline D2D_POINT_2F GetPointPosition() { return D2D_POINT_2F{ mPosition.x, mPosition.y }; }
	inline Fix GetRotation() { return mRotation; }
	inline void SetRotation(Fix rot) { mRotation = rot; }

	virtual eComponentTag GetTag() override { return eComponentTag::Transform; }
};

