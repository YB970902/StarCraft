#include "stdafx.h"
#include "UIGameObject.h"

void UIGameObject::init()
{
	mpTransform = static_cast<TransformComponent*>(AddComponent(new TransformComponent()));
	mpRenderer = static_cast<RendererComponent*>(AddComponent(new UIRendererComponent()));
}
