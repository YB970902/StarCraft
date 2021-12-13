#include "stdafx.h"
#include "Tile.h"

Tile::Tile()
	:GameObject()
{
	Init();
}

Tile::~Tile()
{

}

void Tile::Init()
{
	mpRenderer->SetUnitLayer(eUnitLayer::Terrain);
	mpSprite = static_cast<SpriteComponent*>(AddComponent(new SpriteComponent(eBitmapTag::TILE_PALETTE, 14, 76, D2D1::Point2F(0.0f, 0.0f))));
}

void Tile::Release()
{

}

void Tile::Update()
{

}

void Tile::SetTileSprite(eTileTag tileTag)
{
	switch (tileTag)
	{
	case eTileTag::Dirt:
		mpSprite->SetCurFrame(RANDOM->GetValue(0, DIRT_WIDTH - 1), RANDOM->GetValue((int)tileTag, (int)tileTag + DIRT_HEIGHT - 1));
		break;
	case eTileTag::HeightDirt:
		mpSprite->SetCurFrame(RANDOM->GetValue(0, HEIGHT_DIRT_WIDTH - 1), RANDOM->GetValue((int)tileTag, (int)tileTag + HEIGHT_DIRT_HEIGHT - 1));
		break;
	}
}

void Tile::SetPosition(Fix x, Fix y)
{
	mpTransform->SetPosition(Vector2(x, y));
}
