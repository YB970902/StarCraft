#pragma once
#include "RendererComponent.h"

class UIRendererComponent : public RendererComponent
{
public:
	UIRendererComponent();
	virtual ~UIRendererComponent();
	virtual void Init(GameObject* pObject) override;
	virtual void Release() override;
	virtual void Update() override;
};

