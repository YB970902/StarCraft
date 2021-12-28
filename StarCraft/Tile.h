#pragma once
#include "GameObject.h"

class Tile : public GameObject
{
public:
	static enum class eTileTag
	{
		Dirt = 0b1,
		HeightDirt = 0b10,

		Center_LT = 0b00000000000000000100,
		Center_RT = 0b00000000000000001000,
		Center_LB = 0b00000000000000010000,
		Center_RB = 0b00000000000000100000,

		Left_T = 0b00000000000001000000,
		Left_B = 0b00000000000010000000,

		Right_T = 0b00000000000100000000,
		Right_B = 0b00000000001000000000,

		LeftExtra_T = 0b00000000010000000000,
		LeftExtra_B = 0b00000000100000000000,

		RightExtra_T = 0b00000001000000000000,
		RightExtra_B = 0b00000010000000000000,

		Top_L = 0b00000100000000000000,
		Top_R = 0b00001000000000000000,

		Bottom_L = 0b00010000000000000000,
		Bottom_R = 0b00100000000000000000,

		BottomExtra_L = 0b01000000000000000000,
		BottomExtra_R = 0b10000000000000000000,

		TopExtra_L = 0b0100000000000000000000,
		TopExtra_R = 0b1000000000000000000000,
	};


	static const int TILE_SIZE = 30;

	//static const int GROUND_MAX_COL = 14;
	static const int GROUND_MAX_COL = 12;
	static const int WALL_MAX_COL = 12;

	static const int DIRT = 0;
	static const int HEIGHT_DIRT = 2;

private:
	SpriteComponent* mpSprite = nullptr;

	int mCurTag = (int)eTileTag::Dirt;
	int mCurRow = 0;
	int mCurCol = 0;

public:
	Tile();
	virtual ~Tile();

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;

	void SetTileSprite(int tag, int col, int offset);
	void SetTileRowCol(int row, int col);

	void SetPosition(Fix x, Fix y);
	Vector2 GetPosition() { if (mpTransform) { return mpTransform->GetPosition(); } return Vector2::Zero(); }

	inline int GetCurrentTag() { return (int)mCurTag; }
	inline int GetCurrentRow() { return mCurRow; }
	inline int GetCurrentColumn() { return mCurCol; }
};
