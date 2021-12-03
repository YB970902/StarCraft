#pragma once
#include "Component.h"
#include "EffectData.h"

class EffectComponent : public Component
{
private:
	ID2D1Effect* mpEffect = nullptr;
public:
	EffectComponent(eEffectTag tag);
	virtual ~EffectComponent();

	virtual void Init(GameObject* pObject) override;
	virtual void Release() override;
	virtual void Update() override;

	inline ID2D1Effect* GetEffect() { return mpEffect; }

	virtual eComponentTag GetTag() override { return eComponentTag::Effect; }
};