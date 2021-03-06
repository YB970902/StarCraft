#pragma once
#include "Component.h"

class SpriteComponent;
class EffectComponent;
class TransformComponent;
class RendererComponent : public Component
{
protected:
	eUnitLayer mUnitLayer = eUnitLayer::Ground;

	SpriteComponent* mpSprite = nullptr;
	EffectComponent* mpEffect = nullptr;
	TransformComponent* mpTransform = nullptr;

	D2D_RECT_F mRect = {};

	Vector2 mPrevPosition;

	bool mbIsRender = true;

public:
	RendererComponent();
	virtual ~RendererComponent();
	virtual void Init(GameObject* pObject) override;
	virtual void Release() override;
	virtual void Update() override;
	void Render(ID2D1DeviceContext2* pContext);

	inline D2D_RECT_F GetRectPosition() { return mRect; }
	D2D_POINT_2F GetPosition();

	virtual void LinkComponent(Component* pOther) override;

	inline virtual eComponentTag GetTag() override { return eComponentTag::Renderer; }
	inline eUnitLayer GetUnitLayer() const { return mUnitLayer; }
	void SetUnitLayer(eUnitLayer layer);
	int GetUnitY() const;

	inline void SetIsRender(bool set) { mbIsRender = set; }
	inline bool IsRender() { return mbIsRender; }
};