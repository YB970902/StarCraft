#include "stdafx.h"
#include "UIGameObject.h"

void UIGameObject::Render(ID2D1DeviceContext2* pContext)
{
}

UIGameObject::UIGameObject()
	:GameObject(new UIRendererComponent())
{
	
}

UIGameObject::~UIGameObject()
{
}