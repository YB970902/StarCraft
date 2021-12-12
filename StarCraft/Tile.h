#pragma once
#include "GameObject.h"

class Tile : public GameObject
{
public:
	static enum class eTileTag
	{
		Dirt = 0,
		HeightDirt = 2,
	};

	static const int TILE_SIZE = 31;

private:
	const int DIRT_WIDTH = 14;
	const int DIRT_HEIGHT = 2;

	const int HEIGHT_DIRT_WIDTH = 14;
	const int HEIGHT_DIRT_HEIGHT = 2;

	SpriteComponent* mpSprite = nullptr;

public:
	Tile();
	virtual ~Tile();

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;

	void SetTileSprite(eTileTag tileTag);

	void SetPosition(Fix x, Fix y);
};
