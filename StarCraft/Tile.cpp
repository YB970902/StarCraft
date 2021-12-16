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

void Tile::SetTileSprite(int tag, int col, int offset)
{
	static pair<int, int> arrSpriteIdx[]
	{
		pair<int, int>((int)eTileTag::Dirt, 0),
		pair<int, int>((int)eTileTag::HeightDirt, 2),

		pair<int, int>((int)eTileTag::Center_LT, 4),
		pair<int, int>((int)eTileTag::Center_RT, 8),
		pair<int, int>((int)eTileTag::Center_LB, 12),
		pair<int, int>((int)eTileTag::Center_RB, 18),

		pair<int, int>((int)eTileTag::Left_T, 24),
		pair<int, int>((int)eTileTag::Left_B, 26),

		pair<int, int>((int)eTileTag::Right_T, 30),
		pair<int, int>((int)eTileTag::Right_B, 32),

		pair<int, int>((int)eTileTag::LeftExtra_T, 0),
		pair<int, int>((int)eTileTag::LeftExtra_B, 28),

		pair<int, int>((int)eTileTag::RightExtra_T, 0),
		pair<int, int>((int)eTileTag::RightExtra_B, 34),

		pair<int, int>((int)eTileTag::Top_L, 6),
		pair<int, int>((int)eTileTag::Top_R, 10),

		pair<int, int>((int)eTileTag::Bottom_L, 14),
		pair<int, int>((int)eTileTag::Bottom_R, 20),

		pair<int, int>((int)eTileTag::BottomExtra_L, 16),
		pair<int, int>((int)eTileTag::BottomExtra_R, 22),

		pair<int, int>((int)eTileTag::Center_LB | (int)eTileTag::Top_L, 36),
		pair<int, int>((int)eTileTag::Center_RB | (int)eTileTag::Top_R, 40),

		pair<int, int>((int)eTileTag::Center_LT | (int)eTileTag::Bottom_L, 38),
		pair<int, int>((int)eTileTag::Center_RT | (int)eTileTag::Bottom_R, 42),

		pair<int, int>((int)eTileTag::Left_T | (int)eTileTag::LeftExtra_B, 56),
		pair<int, int>((int)eTileTag::Right_T | (int)eTileTag::RightExtra_B, 58),

		pair<int, int>((int)eTileTag::Top_L | (int)eTileTag::BottomExtra_L, 44),
		pair<int, int>((int)eTileTag::Top_R | (int)eTileTag::BottomExtra_R, 46),

		pair<int, int>((int)eTileTag::Center_LT | (int)eTileTag::BottomExtra_R, 38),
		pair<int, int>((int)eTileTag::Center_RT | (int)eTileTag::BottomExtra_L, 42),

		pair<int, int>((int)eTileTag::Top_L | (int)eTileTag::Bottom_R, 72),
		pair<int, int>((int)eTileTag::Top_R | (int)eTileTag::Bottom_L, 74),

		pair<int, int>((int)eTileTag::Top_L | (int)eTileTag::LeftExtra_B, 48),
		pair<int, int>((int)eTileTag::Top_R | (int)eTileTag::RightExtra_B, 50),

		pair<int, int>((int)eTileTag::BottomExtra_L | (int)eTileTag::LeftExtra_B, 52),
		pair<int, int>((int)eTileTag::BottomExtra_R | (int)eTileTag::RightExtra_B, 54),

		pair<int, int>((int)eTileTag::Left_T | (int)eTileTag::BottomExtra_L, 52),
		pair<int, int>((int)eTileTag::Right_T | (int)eTileTag::BottomExtra_R, 54),

		pair<int, int>((int)eTileTag::Right_B | (int)eTileTag::Left_T, 60),
		pair<int, int>((int)eTileTag::Left_B | (int)eTileTag::Right_T, 62),

		pair<int, int>((int)eTileTag::Center_RB | (int)eTileTag::TopExtra_L, 40),
		pair<int, int>((int)eTileTag::Center_LB | (int)eTileTag::TopExtra_R, 36),
	};

	size_t size = ARRAYSIZE(arrSpriteIdx);

	for (size_t i = 0; i < size; ++i)
	{
		if (arrSpriteIdx[i].first == tag)
		{
			mCurTag = tag;
			mCurCol = col;
			mCurRow = arrSpriteIdx[i].second + offset;
			mpSprite->SetCurFrame(mCurCol, mCurRow);
			return;
		}
	}
}

void Tile::SetPosition(Fix x, Fix y)
{
	mpTransform->SetPosition(Vector2(x, y));
}
