#pragma once
#include "Scene.h"

class RectGizmo;
class TextGizmo;
class LineGizmo;
class Tile;
class MapToolScene : public Scene
{
private:
	const Fix CAMERA_MOVING_SPEED = 500;
	const Fix CAMERA_FAST_MOVING_SPEED = 1000;

	const float UI_WIDTH = 350;

	const float TILE_BTN_WIDTH = 100;
	const float TILE_BTN_HEIGHT = 50;

	const float GROUND_BTN_X = 100;
	const float GROUND_BTN_Y = 200;

	const float WALL_BTN_X = 250;
	const float WALL_BTN_Y = 200;

	const float FONT_SIZE = 20;

	bool mbIsOpenUI = false;

	RectGizmo* mpUIBackground = nullptr;
	RectGizmo* mpBtnGroundTile = nullptr;
	RectGizmo* mpBtnWallTile = nullptr;

	TextGizmo* mpTextGround = nullptr;
	TextGizmo* mpTextWall = nullptr;

	LineGizmo* mpLineLT = nullptr;
	LineGizmo* mpLineLB = nullptr;
	LineGizmo* mpLineRT = nullptr;
	LineGizmo* mpLineRB = nullptr;

	int mMapWidth = 128;
	int mMapHeight = 128;

	vector<Tile*> mVecTile;
public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update() override;

private:
	void SetUIActive(bool set);
};

