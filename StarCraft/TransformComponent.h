#pragma once
#include "Component.h"

class TransformComponent : public Component
{
private:
	TransformComponent* mpParent = nullptr;

	Vector2 mLocalPosition = { 0, 0 };
	Fix mLocalRotation = 0;
	bool mbIsMoved = false;
public:
	TransformComponent();
	virtual ~TransformComponent();

	virtual void Init(GameObject* pObject) override;
	virtual void Release() override;
	virtual void Update() override;

	inline void SetParent(TransformComponent* pParent) { mpParent = pParent; }

	void SetPosition(const Fix& x, const Fix& y);
	void SetPosition(const POINT& pos);
	void SetPosition(const Vector2& pos);
	void AddPosition(const Vector2& add);
	Vector2 GetPosition();

	Vector2 GetLocalPosition() { return mLocalPosition; }
	void SetLocalPosition(const Vector2& pos) { mLocalPosition = pos; }

	Fix GetRotation();
	void SetRotation(const Fix& rot);

	inline bool IsMoved() { return mbIsMoved; }

	virtual eComponentTag GetTag() override { return eComponentTag::Transform; }
};

