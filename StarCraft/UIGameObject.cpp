#include "stdafx.h"
#include "UIGameObject.h"

UIGameObject::UIGameObject()
	:GameObject(new UIRendererComponent())
{
	
}

UIGameObject::~UIGameObject()
{
}