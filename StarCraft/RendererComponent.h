#pragma once
#include "Component.h"

class SpriteComponent;
class EffectComponent;
class TransformComponent;
class RendererComponent : public Component
{
private:
	eUnitLayer mUnitLayer = eUnitLayer::Ground;

	SpriteComponent* mpSprite = nullptr;
	EffectComponent* mpEffect = nullptr;
	TransformComponent* mpTransform = nullptr;

public:
	virtual void Init(GameObject* pObject) override;
	virtual void Release() override;
	virtual void Update() override;
	void Render(ID2D1DeviceContext2* pContext);

	D2D_RECT_F GetRectPosition();
	D2D_POINT_2F GetPosition();

	virtual void LinkComponent(Component* pOther) override;

	inline virtual eComponentTag GetTag() override { return eComponentTag::Renderer; }
	inline eUnitLayer GetUnitLayer() { return mUnitLayer; }
	inline void SetUnitLayer(eUnitLayer layer) { mUnitLayer = layer; }
};