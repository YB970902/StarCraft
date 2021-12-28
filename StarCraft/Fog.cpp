#include "stdafx.h"
#include "Fog.h"

Fog::Fog()
{
	Init();
}

Fog::~Fog()
{
}

void Fog::Init()
{
	mpSprite = static_cast<SpriteComponent*>(AddComponent(new SpriteComponent(eBitmapTag::TILE_FULL_FOG, 6, 1, D2D1::Point2F(0, 0))));
	mpSprite->SetCurFrameX(5);
	//mpEffect = static_cast<EffectComponent*>(AddComponent(new EffectComponent(eEffectTag::GAUSSIAN)))->GetEffect();
	//mpEffect->SetInput(0, mpSprite->GetBitmap());
	//mpEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, 5.0f);

	mpRenderer->SetUnitLayer(eUnitLayer::Fog);
}

void Fog::Release()
{
}

void Fog::Update()
{
}

void Fog::SetFrameX(int frame)
{
	//mpSprite->ChangeBitmap(eBitmapTag::TILE_HALF_FOG);
	//mpEffect->SetInput(0, mpSprite->GetBitmap());
	mpSprite->SetCurFrameX(frame);
}
