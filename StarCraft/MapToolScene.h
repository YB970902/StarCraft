#pragma once
#include "Scene.h"

#define IS_TAG_IN_BIT(tag, bit) ((tag & (bit)) == (tag))

class LineGizmo;
class Tile;
class MapObject;
class MapToolScene : public Scene
{
private:
	const Fix CAMERA_MOVING_SPEED = 500;
	const Fix CAMERA_FAST_MOVING_SPEED = 1000;

	const int BUILDING_HALF_WIDTH = 2;
	const int BUILDING_HALF_HEIGHT = 1;

	LineGizmo* mpLineLT = nullptr;
	LineGizmo* mpLineLB = nullptr;
	LineGizmo* mpLineRT = nullptr;
	LineGizmo* mpLineRB = nullptr;

	int mMapWidth = 64;
	int mMapHeight = 64;

	const int DIAMOND_WIDTH_COUNT = 4;
	const int DIAMOND_HEIGHT_COUNT = 2;

	int mDiamondWidth = 0;
	int mDiamondHeight = 0;

	vector<Tile*> mVecTile;

	POINT mPrevMouseCenter = {};
	POINT mCurMouseCenter = {};

	int mCurTag = 0;

	bool mbIsDrawTerrain = true;

	vector<eBuildingTag> mVecBuildingTag;
	vector<eBitmapTag> mVecDetectTag;
	MapObject* mpCurObject = nullptr;
	eBuildingTag mCurBuildingTag = eBuildingTag::Barrack;

	const float* mArrCurObjectColor = EFFECT_COLOR_GREEN;

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update() override;

private:
	void ProcessTerrain();
	void ProcessBuilding();
	bool IsMouseCollidedDiamond(const POINT& mousePos, const POINT& diaIndex);
	void SetDiamondPosition(const POINT& index);

	void DrawTile();

	void DrawDirt(const POINT& lt, const POINT& rb);

	void DrawWall(const POINT& lt, const POINT& rb);
	void DrawWallCenter(const POINT& lt, const POINT& rb, int col = -1);

	void DrawWallLeft(const POINT& lt, const POINT& rb, int col = -1);
	void DrawWallLeftExtra(const POINT& lt, const POINT& rb, int col = -1);
	void DrawWallRight(const POINT& lt, const POINT& rb, int col = -1);
	void DrawWallRightExtra(const POINT& lt, const POINT& rb, int col = -1);

	void DrawWallTop(const POINT& lt, const POINT& rb, int col = -1);
	void DrawWallBottom(const POINT& lt, const POINT& rb, int col = -1);
	void DrawWallBottomExtra(const POINT& lt, const POINT& rb, int col = -1);

	bool GetTileTag(const POINT& index, int& result);
	bool GetTileColumn(const POINT& index, int& result);
	void ChangeTile(const POINT& index, int tag, int col, int offset);

	void SetIsLineActive(bool set);

	void ChangeToNone();
	void ChangeToBarrack();
	void ChangeToFactory();
	void ChangeToGroup1StartLocation();
	void ChangeToGroup2StartLocation();

	void SaveMap(LPCWSTR path);
};