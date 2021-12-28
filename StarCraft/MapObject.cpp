#include "stdafx.h"
#include "MapObject.h"

MapObject::MapObject(eBitmapTag bitmapTag)
	:GameObject()
{
	mpSprite = static_cast<SpriteComponent*>(AddComponent(new SpriteComponent(bitmapTag)));
	Init();
}

MapObject::~MapObject()
{
}

void MapObject::Init()
{
	mpEffect = static_cast<EffectComponent*>(AddComponent(new EffectComponent(eEffectTag::COLOR_REPLACE)));
	ChangeColor(EFFECT_COLOR_RED);
	mpRenderer->SetUnitLayer(eUnitLayer::Ground);
}

void MapObject::Release()
{
}

void MapObject::Update()
{
}

void MapObject::ChangeColor(const float* arrColor)
{
	if (mpEffect)
	{
		mpEffect->GetEffect()->SetInput(0, mpSprite->GetBitmap());
		mpEffect->GetEffect()->SetValue((int)EffectData::eColorReplaceProperty::GROUP_COLOR,
			D2D_VECTOR_3F{ arrColor[0], arrColor[1], arrColor[2] });
	}
}