#include "stdafx.h"
#include "UISprite.h"

UISprite::UISprite(eBitmapTag bitmapTag, const D2D_POINT_2F& anchor)
{
	mpSprite = static_cast<SpriteComponent*>(AddComponent(new SpriteComponent(bitmapTag, 1, 1, anchor)));
}

UISprite::~UISprite()
{
}

void UISprite::Init()
{
}

void UISprite::Release()
{
}

void UISprite::Update()
{
}
