#include "stdafx.h"
#include "MapToolScene.h"
#include "RenderManager.h"
#include "UIManager.h"
#include "PhysicsManager.h"
#include "RectGizmo.h"
#include "TextGizmo.h"
#include "LineGizmo.h"
#include "Tile.h"
#include "MapObject.h"

void MapToolScene::Enter()
{
	SOUND->StopAll();
	SOUND->Play(eSoundTag::TerranTheme);
	RANDOM->SetSeed(time(nullptr));

	CAMERA->SetMovingSize(POINT{ mMapWidth * Tile::TILE_SIZE, mMapHeight * Tile::TILE_SIZE });
	RENDER->InitLayerSize(mMapWidth * Tile::TILE_SIZE, mMapHeight * Tile::TILE_SIZE);
	PHYSICS->InitLayerSize(mMapWidth * Tile::TILE_SIZE, mMapHeight * Tile::TILE_SIZE);

	UI->Init();

	mpLineLT = (LineGizmo*)UI->CreateLine(Vector2(0, 0), Vector2(Tile::TILE_SIZE * 2, -Tile::TILE_SIZE), 0, 3, D2D1::ColorF::LimeGreen);
	mpLineLB = (LineGizmo*)UI->CreateLine(Vector2(0, 0), Vector2(Tile::TILE_SIZE * 2, Tile::TILE_SIZE), 0, 3, D2D1::ColorF::LimeGreen);
	mpLineRT = (LineGizmo*)UI->CreateLine(Vector2(Tile::TILE_SIZE * 2, -Tile::TILE_SIZE), Vector2(Tile::TILE_SIZE * 4, 0), 0, 3, D2D1::ColorF::LimeGreen);
	mpLineRB = (LineGizmo*)UI->CreateLine(Vector2(Tile::TILE_SIZE * 2, Tile::TILE_SIZE), Vector2(Tile::TILE_SIZE * 4, 0), 0, 3, D2D1::ColorF::LimeGreen);

	mVecTile.resize(mMapWidth * mMapHeight);
	for (int x = 0; x < mMapWidth; ++x)
	{
		for (int y = 0; y < mMapHeight; ++y)
		{
			mVecTile[x + y * mMapWidth] = static_cast<Tile*>(AddGameObject(new Tile));
			mVecTile[x + y * mMapWidth]->SetPosition(x * Tile::TILE_SIZE, y * Tile::TILE_SIZE);
			mVecTile[x + y * mMapWidth]->SetTileSprite((int)Tile::eTileTag::Dirt, RANDOM->GetValue(0, Tile::GROUND_MAX_COL - 1), 0);
		}
	}

	mVecBuildingTag.resize(mMapWidth * mMapHeight);
	mVecDetectTag.resize(mMapWidth * mMapHeight);

	mpCurObject = static_cast<MapObject*>(AddGameObject(new MapObject(eBitmapTag::NONE)));

	mCurTag = (int)Tile::eTileTag::Dirt;

	mDiamondWidth = Tile::TILE_SIZE * DIAMOND_WIDTH_COUNT;
	mDiamondHeight = Tile::TILE_SIZE * DIAMOND_HEIGHT_COUNT;
}

void MapToolScene::Exit()
{
	SOUND->StopAll();
	PHYSICS->Release();

	UI->Release();

	UI->RemoveUI(mpLineLT);
	UI->RemoveUI(mpLineLB);
	UI->RemoveUI(mpLineRT);
	UI->RemoveUI(mpLineRB);
}

void MapToolScene::Update()
{
	if (INPUT->IsOnceKeyDown(VK_BACK))
	{
		SCENE->ChangeScene(eSceneTag::MainScene);
		return;
	}

	if (INPUT->IsOnceKeyDown('1'))
	{
		mCurTag = (int)Tile::eTileTag::Dirt;
		ChangeToNone();
	}
	if (INPUT->IsOnceKeyDown('2'))
	{
		mCurTag = (int)Tile::eTileTag::HeightDirt;
		ChangeToNone();
	}
	if (INPUT->IsOnceKeyDown('3'))
	{
		ChangeToBarrack();
	}
	if (INPUT->IsOnceKeyDown('4'))
	{
		ChangeToFactory();
	}
	if (INPUT->IsOnceKeyDown('5'))
	{
		ChangeToGroup1StartLocation();
	}
	if (INPUT->IsOnceKeyDown('6'))
	{
		ChangeToGroup2StartLocation();
	}
	if (INPUT->IsOnceKeyDown(VK_RETURN))
	{
		SaveMap(TEXT("MapData/Test.txt"));
	}

	if (mbIsDrawTerrain)
	{
		ProcessTerrain();
	}
	else
	{
		ProcessBuilding();
	}

	if (INPUT->IsStayKeyDown(VK_LSHIFT))
	{
		if (INPUT->IsStayKeyDown('A')) { CAMERA->AddPosition(Vector2::Right() * CAMERA_FAST_MOVING_SPEED * DELTA_TIME); }
		if (INPUT->IsStayKeyDown('D')) { CAMERA->AddPosition(Vector2::Left() * CAMERA_FAST_MOVING_SPEED * DELTA_TIME); }
		if (INPUT->IsStayKeyDown('W')) { CAMERA->AddPosition(Vector2::Down() * CAMERA_FAST_MOVING_SPEED * DELTA_TIME); }
		if (INPUT->IsStayKeyDown('S')) { CAMERA->AddPosition(Vector2::Up() * CAMERA_FAST_MOVING_SPEED * DELTA_TIME); }
	}
	else
	{
		if (INPUT->IsStayKeyDown('A')) { CAMERA->AddPosition(Vector2::Right() * CAMERA_MOVING_SPEED * DELTA_TIME); }
		if (INPUT->IsStayKeyDown('D')) { CAMERA->AddPosition(Vector2::Left() * CAMERA_MOVING_SPEED * DELTA_TIME); }
		if (INPUT->IsStayKeyDown('W')) { CAMERA->AddPosition(Vector2::Down() * CAMERA_MOVING_SPEED * DELTA_TIME); }
		if (INPUT->IsStayKeyDown('S')) { CAMERA->AddPosition(Vector2::Up() * CAMERA_MOVING_SPEED * DELTA_TIME); }
	}
}

void MapToolScene::ProcessTerrain()
{
	POINT mousePos = INPUT->GetMousePosition();
	POINT mouseIndex = { mousePos.x / Tile::TILE_SIZE / 2 , mousePos.y / Tile::TILE_SIZE };

	POINT offset1 = { 0, 0 };
	POINT offset2 = { 0, 0 };

	if (mouseIndex.x % 2) { offset2.x += 2; }
	else { offset1.x += 2; }

	if (mouseIndex.y % 2) { ++offset2.y; }
	else { ++offset1.y; }

	POINT diaIndex1 = { mouseIndex.x * 2 + offset1.x, mouseIndex.y + offset1.y };
	POINT diaIndex2 = { mouseIndex.x * 2 + offset2.x, mouseIndex.y + offset2.y };

	mPrevMouseCenter = mCurMouseCenter;

	if (diaIndex1.x == 0 || diaIndex1.y == 0)
	{
		if (IsMouseCollidedDiamond(mousePos, diaIndex2))
		{
			SetDiamondPosition(diaIndex2);
		}
		else
		{
			SetDiamondPosition(diaIndex1);
		}
	}
	else
	{
		if (IsMouseCollidedDiamond(mousePos, diaIndex1))
		{
			SetDiamondPosition(diaIndex1);
		}
		else
		{
			SetDiamondPosition(diaIndex2);
		}
	}

	if (INPUT->IsStayKeyDown(VK_LBUTTON))
	{
		if (INPUT->IsOnceKeyDown(VK_LBUTTON) ||
			(mPrevMouseCenter.x != mCurMouseCenter.x || mPrevMouseCenter.y != mCurMouseCenter.y))
		{
			DrawTile();
		}
	}
}

void MapToolScene::ProcessBuilding()
{
	POINT mousePos = INPUT->GetMousePosition();
	POINT mouseIndex = { mousePos.x / Tile::TILE_SIZE , mousePos.y / Tile::TILE_SIZE };
	mpCurObject->SetPosition(Vector2(mouseIndex.x * Tile::TILE_SIZE, mouseIndex.y * Tile::TILE_SIZE));

	float halfObjectWidth = mpCurObject->GetWidth() * 0.5f;
	float halfObjectHeight = mpCurObject->GetHeight() * 0.5f;
	POINT LT = { mousePos.x / Tile::TILE_SIZE - BUILDING_HALF_WIDTH, mousePos.y / Tile::TILE_SIZE - BUILDING_HALF_HEIGHT };
	POINT LB = { mousePos.x / Tile::TILE_SIZE - BUILDING_HALF_WIDTH, mousePos.y / Tile::TILE_SIZE + BUILDING_HALF_HEIGHT };
	POINT RT = { mousePos.x / Tile::TILE_SIZE + (BUILDING_HALF_WIDTH - 1), mousePos.y / Tile::TILE_SIZE - BUILDING_HALF_HEIGHT };
	POINT RB = { mousePos.x / Tile::TILE_SIZE + (BUILDING_HALF_WIDTH - 1), mousePos.y / Tile::TILE_SIZE + BUILDING_HALF_HEIGHT };

	int width = RB.x - LT.x + 1;
	int height = RB.y - LT.y + 1;

	if (LT.x < 0 || LT.y < 0 || RB.x >= mMapWidth || RB.y >= mMapHeight)
	{
		mpCurObject->ChangeColor(EFFECT_COLOR_BLACK);
	}
	else if (mVecDetectTag[LT.x + LT.y * mMapWidth] == eBitmapTag::NONE &&
		mVecDetectTag[LB.x + LB.y * mMapWidth] == eBitmapTag::NONE &&
		mVecDetectTag[RT.x + RT.y * mMapWidth] == eBitmapTag::NONE &&
		mVecDetectTag[RB.x + RB.y * mMapWidth] == eBitmapTag::NONE)
	{
		for (int x = 0; x < width; ++x)
		{
			for (int y = 0; y < height; ++y)
			{
				int row = mVecTile[(LT.x + x) + (LT.y + y) * mMapWidth]->GetCurrentRow();
				if (row == Tile::DIRT || row == Tile::HEIGHT_DIRT)
				{
					mpCurObject->ChangeColor(EFFECT_COLOR_GREEN);
				}
				else
				{
					mpCurObject->ChangeColor(EFFECT_COLOR_BLACK);
					return;
				}
			}
		}
	}
	else
	{
		mpCurObject->ChangeColor(EFFECT_COLOR_BLACK);
	}

	if (INPUT->IsOnceKeyDown(VK_LBUTTON))
	{

		for (int x = 0; x < width; ++x)
		{
			for (int y = 0; y < height; ++y)
			{
				mVecDetectTag[(LT.x + x) + (LT.y + y) * mMapWidth] = mpCurObject->GetBitmapTag();
			}
		}

		MapObject* pObj = static_cast<MapObject*>(AddGameObject(new MapObject(mpCurObject->GetBitmapTag())));
		pObj->SetMaxFrame(mpCurObject->GetMaxFrameX(), mpCurObject->GetMaxFrameY());
		pObj->SetPosition(Vector2(mouseIndex.x * Tile::TILE_SIZE, mouseIndex.y * Tile::TILE_SIZE));
		pObj->ChangeColor(mArrCurObjectColor);

		mVecBuildingTag[mouseIndex.x + mouseIndex.y * mMapWidth] = mCurBuildingTag;
	}
}

bool MapToolScene::IsMouseCollidedDiamond(const POINT& mousePos, const POINT& diaIndex)
{
	POINT diaCenter = { diaIndex.x * Tile::TILE_SIZE, diaIndex.y * Tile::TILE_SIZE };

	POINT tileCenter = POINT{ mDiamondWidth / 2, mDiamondHeight / 2 };


	if ((fabs(diaCenter.x - mousePos.x) / tileCenter.x + fabs(diaCenter.y - mousePos.y) / tileCenter.y) <= 1)
	{
		return true;
	}
	return false;
}

void MapToolScene::SetDiamondPosition(const POINT& index)
{
	mCurMouseCenter = index;

	POINT diaPos = { index.x * Tile::TILE_SIZE, index.y * Tile::TILE_SIZE };

	mpLineLT->SetStartPosition(Vector2(diaPos.x - Tile::TILE_SIZE * 2, diaPos.y));
	mpLineLT->SetEndPosition(Vector2(diaPos.x, diaPos.y - Tile::TILE_SIZE));
	mpLineLB->SetStartPosition(Vector2(diaPos.x - Tile::TILE_SIZE * 2, diaPos.y));
	mpLineLB->SetEndPosition(Vector2(diaPos.x, diaPos.y + Tile::TILE_SIZE));

	mpLineRT->SetStartPosition(Vector2(diaPos.x, diaPos.y - Tile::TILE_SIZE));
	mpLineRT->SetEndPosition(Vector2(diaPos.x + Tile::TILE_SIZE * 2, diaPos.y));
	mpLineRB->SetStartPosition(Vector2(diaPos.x, diaPos.y + Tile::TILE_SIZE));
	mpLineRB->SetEndPosition(Vector2(diaPos.x + Tile::TILE_SIZE * 2, diaPos.y));
}

void MapToolScene::DrawTile()
{
	POINT drawLT = { mCurMouseCenter.x - 2, mCurMouseCenter.y - 1 };
	POINT drawRB = { mCurMouseCenter.x + 1, mCurMouseCenter.y };

	switch ((Tile::eTileTag)mCurTag)
	{
	case Tile::eTileTag::Dirt:
		DrawDirt(drawLT, drawRB);
		break;
	case Tile::eTileTag::HeightDirt:
		DrawWall(drawLT, drawRB);
		break;
	}
}

void MapToolScene::DrawDirt(const POINT& lt, const POINT& rb)
{
	for (int x = 0; x < DIAMOND_WIDTH_COUNT; ++x)
	{
		for (int y = 0; y < DIAMOND_HEIGHT_COUNT; ++y)
		{
			ChangeTile(POINT{ lt.x + x, lt.y + y }, mCurTag, RANDOM->GetValue(0, Tile::GROUND_MAX_COL), 0);
		}
	}
}

void MapToolScene::DrawWall(const POINT& lt, const POINT& rb)
{
	DrawWallCenter(lt, rb);

	DrawWallLeft(lt, rb);
	DrawWallLeftExtra(lt, rb);
	DrawWallRight(lt, rb);
	DrawWallRightExtra(lt, rb);

	DrawWallTop(lt, rb);
	DrawWallBottom(lt, rb);
	DrawWallBottomExtra(lt, rb);
}

void MapToolScene::DrawWallCenter(const POINT& lt, const POINT& rb, int col)
{
	POINT index = { lt.x, lt.y };
	int tag;

	static const int deltaTag[] = {
		(int)Tile::eTileTag::Center_LT,
		(int)Tile::eTileTag::Center_RT,
		(int)Tile::eTileTag::Center_LB,
		(int)Tile::eTileTag::Center_RB,

		(int)Tile::eTileTag::Top_R,
		(int)Tile::eTileTag::Top_L,
		(int)Tile::eTileTag::Bottom_R,
		(int)Tile::eTileTag::Bottom_L,

		(int)Tile::eTileTag::Dirt,
		(int)Tile::eTileTag::Dirt,
		(int)Tile::eTileTag::BottomExtra_L,
		(int)Tile::eTileTag::BottomExtra_R,

		(int)Tile::eTileTag::Left_B,
		(int)Tile::eTileTag::Right_B,
		(int)Tile::eTileTag::Left_T,
		(int)Tile::eTileTag::Right_T,

		(int)Tile::eTileTag::Right_T,
		(int)Tile::eTileTag::Left_T,
		(int)Tile::eTileTag::Right_B,
		(int)Tile::eTileTag::Left_B,

		(int)Tile::eTileTag::Center_RB,
		(int)Tile::eTileTag::Center_LB,
		(int)Tile::eTileTag::Center_RT,
		(int)Tile::eTileTag::Center_LT,

		(int)Tile::eTileTag::LeftExtra_T,
		(int)Tile::eTileTag::RightExtra_T,
		(int)Tile::eTileTag::LeftExtra_B,
		(int)Tile::eTileTag::RightExtra_B,

		(int)Tile::eTileTag::Center_LT | (int)Tile::eTileTag::Bottom_L,
		(int)Tile::eTileTag::Center_RT | (int)Tile::eTileTag::Bottom_R,
		(int)Tile::eTileTag::Center_LB | (int)Tile::eTileTag::Top_L,
		(int)Tile::eTileTag::Center_RB | (int)Tile::eTileTag::Top_R,
	};

	static const int deltaOffset[] = {
		0, 0,
		2, 0,
		0, 1,
		2, 1,
	};

	size_t size = ARRAYSIZE(deltaOffset);

	for (size_t i = 0; i < size; ++i)
	{
		index = POINT{ lt.x + deltaOffset[i * 2], lt.y + deltaOffset[i * 2 + 1] };

		if (GetTileTag(index, tag))
		{
			int randWall = (col < 0) ? RANDOM->GetValue(0, Tile::WALL_MAX_COL - 1) : col;
			if (IS_TAG_IN_BIT(deltaTag[i + 20], tag))
			{
				ChangeTile(index, (int)Tile::eTileTag::HeightDirt, RANDOM->GetValue(0, Tile::GROUND_MAX_COL - 1), 0);
				ChangeTile(POINT{ index.x + 1, index.y }, (int)Tile::eTileTag::HeightDirt, RANDOM->GetValue(0, Tile::GROUND_MAX_COL - 1), 1);
			}
			else if (deltaTag[i + 28] == tag)
			{
				ChangeTile(index, deltaTag[i + 28], randWall, 0);
				ChangeTile(POINT{ index.x + 1, index.y }, deltaTag[i + 28], randWall, 1);
			}
			else if (tag == (int)Tile::eTileTag::Dirt ||
				IS_TAG_IN_BIT(deltaTag[i], tag) ||
				IS_TAG_IN_BIT(deltaTag[i + 4], tag) ||
				IS_TAG_IN_BIT(deltaTag[i + 8], tag) ||
				IS_TAG_IN_BIT(deltaTag[i + 12], tag) ||
				IS_TAG_IN_BIT(deltaTag[i + 16], tag) ||
				IS_TAG_IN_BIT(deltaTag[i + 24], tag))
			{
				ChangeTile(index, deltaTag[i], randWall, 0);
				ChangeTile(POINT{ index.x + 1, index.y }, deltaTag[i], randWall, 1);
			}
			else
			{
				ChangeTile(index, (int)Tile::eTileTag::HeightDirt, RANDOM->GetValue(0, Tile::GROUND_MAX_COL - 1), 0);
				ChangeTile(POINT{ index.x + 1, index.y }, (int)Tile::eTileTag::HeightDirt, RANDOM->GetValue(0, Tile::GROUND_MAX_COL - 1), 1);
			}
		}
	}
}

void MapToolScene::DrawWallLeft(const POINT& lt, const POINT& rb, int col)
{
	POINT index = { lt.x, lt.y };
	int tag;

	static const int deltaTag[] = {
		(int)Tile::eTileTag::Left_T,
		(int)Tile::eTileTag::Left_B,
	};

	static const int deltaOtherTag[] = {
		(int)Tile::eTileTag::Center_RT,
		(int)Tile::eTileTag::Right_B,
		(int)Tile::eTileTag::LeftExtra_B,
		(int)Tile::eTileTag::Center_RB,

		(int)Tile::eTileTag::Center_RB,
		(int)Tile::eTileTag::Right_T,
		(int)Tile::eTileTag::LeftExtra_T,
		(int)Tile::eTileTag::Center_RT,
	};

	static const int deltaRightColTag[] = {
		(int)Tile::eTileTag::Top_R,
		(int)Tile::eTileTag::Right_B | (int)Tile::eTileTag::Left_T,
		(int)Tile::eTileTag::Bottom_L,
		(int)Tile::eTileTag::BottomExtra_L,

		(int)Tile::eTileTag::Top_L,
		(int)Tile::eTileTag::Left_B | (int)Tile::eTileTag::Right_T,
		(int)Tile::eTileTag::Bottom_R,
		(int)Tile::eTileTag::BottomExtra_R,
	};

	static const int deltaOffset[] = {
		-2, 0,
		-2, 1,
	};

	static const int deltaNearNodeOffset[] = {
		0, -1,
		0, 1,
	};

	static const int deltaLTOffset[] = {
		0, -1,
		0, 0
	};

	static const int deltaRBOffset[] = {
		2, 0,
		2, 1,
	};

	size_t size = ARRAYSIZE(deltaTag);

	int ltTag = 0;
	int lbTag = 0;

	if (GetTileTag(POINT{ lt.x - 4, lt.y}, ltTag) &&
		GetTileTag(POINT{ lt.x - 4, lt.y + 1}, lbTag))
	{
		if (IS_TAG_IN_BIT((int)Tile::eTileTag::Center_RB, ltTag) &&
			IS_TAG_IN_BIT((int)Tile::eTileTag::Center_RT, lbTag))
		{
			DrawWall(POINT{ lt.x - 4, lt.y }, POINT{ lt.x - 2, lt.y + 1 });
		}
	}

	for (size_t i = 0; i < size; ++i)
	{
		index = POINT{ lt.x + deltaOffset[i * 2], lt.y + deltaOffset[i * 2 + 1] };

		if (GetTileTag(index, tag))
		{
			int randWall = (col < 0) ? RANDOM->GetValue(0, Tile::WALL_MAX_COL - 1) : col;

			if (tag == deltaOtherTag[i * 4])
			{
				// 아무것도 안그림
			}
			else if (IS_TAG_IN_BIT(deltaOtherTag[i * 4 + 1], tag))
			{
				int tempTag = 0;
				if (GetTileTag(POINT{ index.x + deltaNearNodeOffset[i * 2], index.y + deltaNearNodeOffset[i * 2 + 1] }, tempTag) && IS_TAG_IN_BIT(deltaOtherTag[i * 4 + 3], tempTag))
				{
					DrawWall(POINT{ index.x + deltaLTOffset[i * 2], index.y + deltaLTOffset[i * 2 + 1] }, POINT{ index.x + deltaRBOffset[i * 2], index.y + deltaRBOffset[i * 2 + 1] });
				}
				else
				{
					ChangeTile(POINT{ index.x, index.y - 1 }, deltaRightColTag[i * 4], randWall, 0);
					ChangeTile(POINT{ index.x + 1, index.y - 1 }, deltaRightColTag[i * 4], randWall, 1);
					ChangeTile(index, deltaRightColTag[i * 4 + 1], randWall, 0);
					ChangeTile(POINT{ index.x + 1, index.y }, deltaRightColTag[i * 4 + 1], randWall, 1);
					ChangeTile(POINT{ index.x, index.y + 1 }, deltaRightColTag[i * 4 + 2], randWall, 0);
					ChangeTile(POINT{ index.x + 1, index.y + 1 }, deltaRightColTag[i * 4 + 2], randWall, 1);
					ChangeTile(POINT{ index.x, index.y + 2 }, deltaRightColTag[i * 4 + 3], randWall, 0);
					ChangeTile(POINT{ index.x + 1, index.y + 2 }, deltaRightColTag[i * 4 + 3], randWall, 1);
				}
			}
			else if (tag == (int)Tile::eTileTag::Dirt || tag == deltaOtherTag[i * 4 + 2])
			{
				ChangeTile(index, deltaTag[i], randWall, 0);
				ChangeTile(POINT{ index.x + 1, index.y }, deltaTag[i], randWall, 1);
			}
		}
	}
}

void MapToolScene::DrawWallLeftExtra(const POINT& lt, const POINT& rb, int col)
{
	POINT index = { lt.x, lt.y };
	int tag;

	static const int deltaTag[] = {
		(int)Tile::eTileTag::LeftExtra_T,
		(int)Tile::eTileTag::LeftExtra_B,
	};

	static const int deltaOtherTag[] = {
		(int)Tile::eTileTag::Center_RB,
		(int)Tile::eTileTag::Dirt,

		(int)Tile::eTileTag::Center_RT,
		(int)Tile::eTileTag::RightExtra_T,
	};

	static const int deltaOffset[] = {
		-2, -1,
		-2, 2,
	};

	static const int deltaLTOffset[] = {
		0, 0,
		0, -1,
	};

	static const int deltaRBOffset[] = {
		2, 1,
		2, 0,
	};

	size_t size = ARRAYSIZE(deltaTag);

	for (size_t i = 0; i < size; ++i)
	{
		index = POINT{ lt.x + deltaOffset[i * 2], lt.y + deltaOffset[i * 2 + 1] };

		if (GetTileTag(index, tag))
		{
			int randWall = (col < 0) ? RANDOM->GetValue(0, Tile::WALL_MAX_COL - 1) : col;

			if (IS_TAG_IN_BIT(deltaOtherTag[i * 2], tag))
			{
				DrawWall(POINT{ index.x + deltaLTOffset[i * 2], index.y + deltaLTOffset[i * 2 + 1] },
					POINT{ index.x + deltaRBOffset[i * 2], index.y + deltaRBOffset[i * 2 + 1] });
			}
			else if (tag == (int)Tile::eTileTag::Dirt || tag == deltaOtherTag[i * 2 + 1])
			{
				ChangeTile(index, deltaTag[i], randWall, 0);
				ChangeTile(POINT{ index.x + 1, index.y }, deltaTag[i], randWall, 1);
			}
		}
	}
}

void MapToolScene::DrawWallRight(const POINT& lt, const POINT& rb, int col)
{
	POINT index = { lt.x, lt.y };
	int tag;

	static const int deltaTag[] = {
		(int)Tile::eTileTag::Right_T,
		(int)Tile::eTileTag::Right_B,
	};

	static const int deltaOtherTag[] = {
		(int)Tile::eTileTag::Center_LT,
		(int)Tile::eTileTag::Left_B,
		(int)Tile::eTileTag::RightExtra_B,
		(int)Tile::eTileTag::Center_LB,

		(int)Tile::eTileTag::Center_LB,
		(int)Tile::eTileTag::Left_T,
		(int)Tile::eTileTag::RightExtra_T,
		(int)Tile::eTileTag::Center_LT,
	};

	static const int deltaRightColTag[] = {
		(int)Tile::eTileTag::Top_L,
		(int)Tile::eTileTag::Left_B | (int)Tile::eTileTag::Right_T,
		(int)Tile::eTileTag::Bottom_R,
		(int)Tile::eTileTag::BottomExtra_R,

		(int)Tile::eTileTag::Top_R,
		(int)Tile::eTileTag::Right_B | (int)Tile::eTileTag::Left_T,
		(int)Tile::eTileTag::Bottom_L,
		(int)Tile::eTileTag::BottomExtra_L,
	};

	static const int deltaOffset[] = {
		4, 0,
		4, 1,
	};

	static const int deltaNearNodeOffset[] = {
		0, -1,
		0, 1,
	};

	static const int deltaLTOffset[] = {
		-2, -1,
		-2, 0
	};

	static const int deltaRBOffset[] = {
		0, 0,
		0, 1,
	};

	size_t size = ARRAYSIZE(deltaTag);

	int ltTag = 0;
	int lbTag = 0;

	if (GetTileTag(POINT{ lt.x + 6, lt.y }, ltTag) &&
		GetTileTag(POINT{ lt.x + 6, lt.y + 1 }, lbTag))
	{
		if (IS_TAG_IN_BIT((int)Tile::eTileTag::Center_LB, ltTag) &&
			IS_TAG_IN_BIT((int)Tile::eTileTag::Center_LT, lbTag))
		{
			DrawWall(POINT{ lt.x + 4, lt.y }, POINT{ lt.x + 6, lt.y + 1 });
		}
	}

	for (size_t i = 0; i < size; ++i)
	{
		index = POINT{ lt.x + deltaOffset[i * 2], lt.y + deltaOffset[i * 2 + 1] };

		if (GetTileTag(index, tag))
		{
			int randWall = (col < 0) ? RANDOM->GetValue(0, Tile::WALL_MAX_COL - 1) : col;

			if (tag == deltaOtherTag[i * 4])
			{
				// 아무것도 안그림
			}
			else if (tag == deltaOtherTag[i * 4 + 1])
			{
				int tempTag = 0;
				if (GetTileTag(POINT{ index.x + deltaNearNodeOffset[i * 2], index.y + deltaNearNodeOffset[i * 2 + 1] }, tempTag) && IS_TAG_IN_BIT(deltaOtherTag[i * 4 + 3], tempTag))
				{
					DrawWall(POINT{ index.x + deltaLTOffset[i * 2], index.y + deltaLTOffset[i * 2 + 1] }, POINT{ index.x + deltaRBOffset[i * 2], index.y + deltaRBOffset[i * 2 + 1] });
				}
				else
				{
					ChangeTile(POINT{ index.x, index.y - 1 }, deltaRightColTag[i * 4], randWall, 0);
					ChangeTile(POINT{ index.x + 1, index.y - 1 }, deltaRightColTag[i * 4], randWall, 1);
					ChangeTile(index, deltaRightColTag[i * 4 + 1], randWall, 0);
					ChangeTile(POINT{ index.x + 1, index.y }, deltaRightColTag[i * 4 + 1], randWall, 1);
					ChangeTile(POINT{ index.x, index.y + 1 }, deltaRightColTag[i * 4 + 2], randWall, 0);
					ChangeTile(POINT{ index.x + 1, index.y + 1 }, deltaRightColTag[i * 4 + 2], randWall, 1);
					ChangeTile(POINT{ index.x, index.y + 2 }, deltaRightColTag[i * 4 + 3], randWall, 0);
					ChangeTile(POINT{ index.x + 1, index.y + 2 }, deltaRightColTag[i * 4 + 3], randWall, 1);
				}
			}
			else if (tag == (int)Tile::eTileTag::Dirt || tag == deltaOtherTag[i * 4 + 2])
			{
				ChangeTile(index, deltaTag[i], randWall, 0);
				ChangeTile(POINT{ index.x + 1, index.y }, deltaTag[i], randWall, 1);
			}
		}
	}
}

void MapToolScene::DrawWallRightExtra(const POINT& lt, const POINT& rb, int col)
{
	POINT index = { lt.x, lt.y };
	int tag;

	static const int deltaTag[] = {
		(int)Tile::eTileTag::RightExtra_T,
		(int)Tile::eTileTag::RightExtra_B,
	};

	static const int deltaOtherTag[] = {
		(int)Tile::eTileTag::Center_LB,
		(int)Tile::eTileTag::Dirt,

		(int)Tile::eTileTag::Center_LT,
		(int)Tile::eTileTag::LeftExtra_T,
	};

	static const int deltaOffset[] = {
		4, -1,
		4, 2,
	};

	static const int deltaLTOffset[] = {
		-2, 0,
		-2, -1,
	};

	static const int deltaRBOffset[] = {
		0, 1,
		0, 0,
	};

	size_t size = ARRAYSIZE(deltaTag);

	for (size_t i = 0; i < size; ++i)
	{
		index = POINT{ lt.x + deltaOffset[i * 2], lt.y + deltaOffset[i * 2 + 1] };

		if (GetTileTag(index, tag))
		{
			int randWall = (col < 0) ? RANDOM->GetValue(0, Tile::WALL_MAX_COL - 1) : col;

			if (IS_TAG_IN_BIT(deltaOtherTag[i * 2], tag))
			{
				DrawWall(POINT{ index.x + deltaLTOffset[i * 2], index.y + deltaLTOffset[i * 2 + 1] },
					POINT{ index.x + deltaRBOffset[i * 2], index.y + deltaRBOffset[i * 2 + 1] });
			}
			else if (tag == (int)Tile::eTileTag::Dirt || tag == deltaOtherTag[i * 2 + 1])
			{
				ChangeTile(index, deltaTag[i], randWall, 0);
				ChangeTile(POINT{ index.x + 1, index.y }, deltaTag[i], randWall, 1);
			}
		}
	}
}

void MapToolScene::DrawWallTop(const POINT& lt, const POINT& rb, int col)
{
	POINT index = { lt.x, lt.y };
	int tag;

	static const int deltaTag[] = {
		(int)Tile::eTileTag::Top_L,
		(int)Tile::eTileTag::Top_R,
	};

	static const int deltaOtherTag[] = {
		(int)Tile::eTileTag::Center_LB,
		(int)Tile::eTileTag::Bottom_R,
		(int)Tile::eTileTag::BottomExtra_L,
		(int)Tile::eTileTag::RightExtra_T,

		(int)Tile::eTileTag::Center_RB,
		(int)Tile::eTileTag::Bottom_L,
		(int)Tile::eTileTag::BottomExtra_R,
		(int)Tile::eTileTag::LeftExtra_T,
	};

	static const int deltaCenterColTag[] = {
		(int)Tile::eTileTag::Top_L | (int)Tile::eTileTag::Center_LB,
		(int)Tile::eTileTag::LeftExtra_B,
		(int)Tile::eTileTag::Center_LT | (int)Tile::eTileTag::Bottom_L,

		(int)Tile::eTileTag::Top_R | (int)Tile::eTileTag::Center_RB,
		(int)Tile::eTileTag::RightExtra_B,
		(int)Tile::eTileTag::Center_RT | (int)Tile::eTileTag::Bottom_R,
	};

	static const int deltaSideColTag[] = {
		(int)Tile::eTileTag::Top_L | (int)Tile::eTileTag::Bottom_R,
		(int)Tile::eTileTag::Top_R | (int)Tile::eTileTag::RightExtra_B,
		(int)Tile::eTileTag::Center_LT | (int)Tile::eTileTag::BottomExtra_R,
		(int)Tile::eTileTag::Left_T| (int)Tile::eTileTag::BottomExtra_L,
		(int)Tile::eTileTag::Center_RB | (int)Tile::eTileTag::TopExtra_L,

		(int)Tile::eTileTag::Top_R | (int)Tile::eTileTag::Bottom_L,
		(int)Tile::eTileTag::Top_L | (int)Tile::eTileTag::LeftExtra_B,
		(int)Tile::eTileTag::Center_RT | (int)Tile::eTileTag::BottomExtra_L,
		(int)Tile::eTileTag::Right_T | (int)Tile::eTileTag::BottomExtra_R,
		(int)Tile::eTileTag::Center_LB | (int)Tile::eTileTag::TopExtra_R,
	};

	static const int deltaUpColTag[] = {
		(int)Tile::eTileTag::Top_L | (int)Tile::eTileTag::BottomExtra_L,
		(int)Tile::eTileTag::Top_R | (int)Tile::eTileTag::BottomExtra_R,
	};

	static const int deltaOtherSideColTag[] = {
		(int)Tile::eTileTag::Left_T,
		(int)Tile::eTileTag::BottomExtra_L,

		(int)Tile::eTileTag::Right_T,
		(int)Tile::eTileTag::BottomExtra_R,
	};

	static const int deltaOffset[] = {
		0, -1,
		2, -1,
	};

	static const int deltaDirection[] = {
		-1, 1
	};

	size_t size = ARRAYSIZE(deltaTag);

	for (size_t i = 0; i < size; ++i)
	{
		index = POINT{ lt.x + deltaOffset[i * 2], lt.y + deltaOffset[i * 2 + 1] };

		if (GetTileTag(index, tag))
		{
			int randWall = (col < 0) ? RANDOM->GetValue(0, Tile::WALL_MAX_COL - 1) : col;

			if (tag == deltaOtherTag[i * 4])
			{
				ChangeTile(index, deltaCenterColTag[i * 3], randWall, 0);
				ChangeTile(POINT{ index.x + 1, index.y }, deltaCenterColTag[i * 3], randWall, 1);
				ChangeTile(POINT{ index.x, index.y + 1 }, deltaCenterColTag[i * 3 + 2], randWall, 0);
				ChangeTile(POINT{ index.x + 1, index.y + 1 }, deltaCenterColTag[i * 3 + 2], randWall, 1);
				int tempTag = 0;
				if (GetTileTag(POINT{ index.x + deltaDirection[i] * 2, index.y + 1 }, tempTag))
				{
					if (IS_TAG_IN_BIT(deltaOtherSideColTag[i * 2], tempTag) || tempTag == deltaOtherSideColTag[i * 2 + 1])
					{
						int tempCol = 0;
						if (GetTileColumn(POINT{ index.x + deltaDirection[i] * 2, index.y }, tempCol))
						{
							ChangeTile(POINT{ index.x + deltaDirection[i] * 2, index.y + 1 }, tempTag | deltaCenterColTag[i * 3 + 1], tempCol, 0);
							ChangeTile(POINT{ index.x + deltaDirection[i] * 2 + 1, index.y + 1 }, tempTag | deltaCenterColTag[i * 3 + 1], tempCol, 1);
						}
						else
						{
							ChangeTile(POINT{ index.x + deltaDirection[i] * 2, index.y + 1 }, tempTag | deltaCenterColTag[i * 3 + 1], randWall, 0);
							ChangeTile(POINT{ index.x + deltaDirection[i] * 2 + 1, index.y + 1 }, tempTag | deltaCenterColTag[i * 3 + 1], randWall, 1);
						}
					}
					else
					{
						if (i == 0)
						{
							DrawWallCenter(POINT{ index.x + deltaDirection[i] * 2, index.y }, POINT{ index.x, index.y + 1 });
							DrawWallLeft(POINT{ index.x + deltaDirection[i] * 2, index.y }, POINT{ index.x, index.y + 1 });
							DrawWallBottom(POINT{ index.x + deltaDirection[i] * 2, index.y }, POINT{ index.x, index.y + 1 });
							DrawWallBottomExtra(POINT{ index.x + deltaDirection[i] * 2, index.y }, POINT{ index.x, index.y + 1 });
						}
						else
						{
							DrawWallCenter(index, POINT{ index.x + deltaDirection[i] * 2, index.y + 1 });
							DrawWallRight(index, POINT{ index.x + deltaDirection[i] * 2, index.y + 1 });
							DrawWallBottom(index, POINT{ index.x + deltaDirection[i] * 2, index.y + 1 });
							DrawWallBottomExtra(index, POINT{ index.x + deltaDirection[i] * 2, index.y + 1 });
						}
					}
				}
			}
			else if (tag == deltaOtherTag[i * 4 + 1])
			{
				int tempCol = 0;
				ChangeTile(index, deltaSideColTag[i * 5], randWall, 0);
				ChangeTile(POINT{ index.x + 1, index.y }, deltaSideColTag[i * 5], randWall, 1);
				if (GetTileColumn(POINT{ index.x + deltaDirection[i] * -2, index.y + 1 }, tempCol))
				{
					ChangeTile(POINT{ index.x + deltaDirection[i] * -2, index.y }, deltaSideColTag[i * 5 + 1], tempCol, 0);
					ChangeTile(POINT{ index.x + deltaDirection[i] * -2 + 1, index.y }, deltaSideColTag[i * 5 + 1], tempCol, 1);
				}
				else
				{
					ChangeTile(POINT{ index.x + deltaDirection[i] * -2, index.y }, deltaSideColTag[i * 5 + 1], randWall, 0);
					ChangeTile(POINT{ index.x + deltaDirection[i] * -2 + 1, index.y }, deltaSideColTag[i * 5 + 1], randWall, 1);
				}
				ChangeTile(POINT{ index.x, index.y + 1 }, deltaSideColTag[i * 5 + 2], randWall, 0);
				ChangeTile(POINT{ index.x + 1, index.y + 1 }, deltaSideColTag[i * 5 + 2], randWall, 1);
				if (GetTileColumn(POINT{ index.x + deltaDirection[i] * 2, index.y - 1 }, tempCol))
				{
					ChangeTile(POINT{ index.x + deltaDirection[i] * 2, index.y + 1 }, deltaSideColTag[i * 5 + 3], tempCol, 0);
					ChangeTile(POINT{ index.x + deltaDirection[i] * 2 + 1, index.y + 1 }, deltaSideColTag[i * 5 + 3], tempCol, 1);
				}
				else
				{
					ChangeTile(POINT{ index.x + deltaDirection[i] * 2, index.y + 1 }, deltaSideColTag[i * 5 + 3], randWall, 0);
					ChangeTile(POINT{ index.x + deltaDirection[i] * 2 + 1, index.y + 1 }, deltaSideColTag[i * 5 + 3], randWall, 1);
				}
				ChangeTile(POINT{ index.x, index.y - 1 }, deltaSideColTag[i * 5 + 4], randWall, 0);
				ChangeTile(POINT{ index.x + 1, index.y - 1 }, deltaSideColTag[i * 5 + 4], randWall, 1);
			}
			else if(tag == deltaOtherTag[i * 4 + 2])
			{
				int col = 0;
				if (GetTileColumn(index, col))
				{
					ChangeTile(index, deltaUpColTag[i], col, 0);
					ChangeTile(POINT{ index.x + 1, index.y }, deltaUpColTag[i], col, 1);
				}
				else
				{
					ChangeTile(index, deltaUpColTag[i], randWall, 0);
					ChangeTile(POINT{ index.x + 1, index.y }, deltaUpColTag[i], randWall, 1);
				}
			}
			else if(tag == (int)Tile::eTileTag::Dirt || tag == deltaTag[i] || tag == deltaOtherTag[i * 4 + 3])
			{
				int col = 0;
				if (GetTileColumn(POINT{ index.x, index.y + 1 }, col))
				{
					ChangeTile(index, deltaTag[i], col, 0);
					ChangeTile(POINT{ index.x + 1, index.y }, deltaTag[i], col, 1);
				}
				else
				{
					ChangeTile(index, deltaTag[i], randWall, 0);
					ChangeTile(POINT{ index.x + 1, index.y }, deltaTag[i], randWall, 1);
				}
			}
		}
	}
}

void MapToolScene::DrawWallBottom(const POINT& lt, const POINT& rb, int col)
{
	POINT index = { lt.x, lt.y };
	int tag;

	static const int deltaTag[] = {
		(int)Tile::eTileTag::Bottom_L,
		(int)Tile::eTileTag::Bottom_R,
	};

	static const int deltaOtherTag[] = {
		(int)Tile::eTileTag::Center_LT,
		(int)Tile::eTileTag::Top_R,
		(int)Tile::eTileTag::BottomExtra_R,
		(int)Tile::eTileTag::Left_B,
		(int)Tile::eTileTag::LeftExtra_T,
		(int)Tile::eTileTag::RightExtra_B,
		(int)Tile::eTileTag::RightExtra_T,

		(int)Tile::eTileTag::Center_RT,
		(int)Tile::eTileTag::Top_L,
		(int)Tile::eTileTag::BottomExtra_L,
		(int)Tile::eTileTag::Right_B,
		(int)Tile::eTileTag::RightExtra_T,
		(int)Tile::eTileTag::LeftExtra_B,
		(int)Tile::eTileTag::LeftExtra_T,
	};

	static const int deltaCenterColTag[] = {
		(int)Tile::eTileTag::Bottom_L | (int)Tile::eTileTag::Center_LT,
		(int)Tile::eTileTag::LeftExtra_B,
		(int)Tile::eTileTag::Center_LB | (int)Tile::eTileTag::Top_L,

		(int)Tile::eTileTag::Bottom_R | (int)Tile::eTileTag::Center_RT,
		(int)Tile::eTileTag::RightExtra_B,
		(int)Tile::eTileTag::Center_RB | (int)Tile::eTileTag::Top_R,
	};

	static const int deltaSideColTag[] = {
		(int)Tile::eTileTag::Bottom_L | (int)Tile::eTileTag::Top_R,
		(int)Tile::eTileTag::LeftExtra_B | (int)Tile::eTileTag::Top_L,
		(int)Tile::eTileTag::BottomExtra_L | (int)Tile::eTileTag::Center_RT,
		(int)Tile::eTileTag::Right_T | (int)Tile::eTileTag::BottomExtra_R,
		(int)Tile::eTileTag::Center_LB | (int)Tile::eTileTag::TopExtra_R,


		(int)Tile::eTileTag::Bottom_R | (int)Tile::eTileTag::Top_L,
		(int)Tile::eTileTag::RightExtra_B | (int)Tile::eTileTag::Top_R,
		(int)Tile::eTileTag::BottomExtra_R | (int)Tile::eTileTag::Center_LT,
		(int)Tile::eTileTag::Left_T | (int)Tile::eTileTag::BottomExtra_L,
		(int)Tile::eTileTag::Center_RB | (int)Tile::eTileTag::TopExtra_L,
	};

	static const int deltaOtherSideColTag[] = {
		(int)Tile::eTileTag::Left_T,
		(int)Tile::eTileTag::Top_L,
		(int)Tile::eTileTag::Center_LT,

		(int)Tile::eTileTag::Right_T,
		(int)Tile::eTileTag::Top_R,
		(int)Tile::eTileTag::Center_RT,
	};

	static const int deltaOffset[] = {
		0, 2,
		2, 2,
	};

	static const int deltaDirection[] = {
		-1, 1
	};

	size_t size = ARRAYSIZE(deltaTag);

	for (size_t i = 0; i < size; ++i)
	{
		index = POINT{ lt.x + deltaOffset[i * 2], lt.y + deltaOffset[i * 2 + 1] };

		if (GetTileTag(index, tag))
		{
			int randWall = 0;
			if (col < 0)
			{
				if (!GetTileColumn(POINT{ index.x, index.y - 1 }, randWall))
				{
					randWall = RANDOM->GetValue(0, Tile::WALL_MAX_COL - 1);
				}
			}
			else { randWall = col; }

			if (tag == deltaOtherTag[i * 7])
			{
				ChangeTile(index, deltaCenterColTag[i * 3], randWall, 0);
				ChangeTile(POINT{ index.x + 1, index.y }, deltaCenterColTag[i * 3], randWall, 1);
				ChangeTile(POINT{ index.x, index.y - 1 }, deltaCenterColTag[i * 3 + 2], randWall, 0);
				ChangeTile(POINT{ index.x + 1, index.y - 1 }, deltaCenterColTag[i * 3 + 2], randWall, 1);
				int tempTag = 0;
				if (GetTileTag(POINT{ index.x + deltaDirection[i] * 2, index.y }, tempTag))
				{
					if (IS_TAG_IN_BIT(deltaOtherSideColTag[i * 3], tempTag) || IS_TAG_IN_BIT(deltaOtherSideColTag[i * 3 + 1], tempTag))
					{
						int tempCol = 0;
						if (GetTileColumn(POINT{ index.x + deltaDirection[i] * 2, index.y - 1 }, tempCol))
						{
							ChangeTile(POINT{ index.x + deltaDirection[i] * 2, index.y }, deltaCenterColTag[i * 3 + 1], tempCol, 0);
							ChangeTile(POINT{ index.x + deltaDirection[i] * 2 + 1, index.y }, deltaCenterColTag[i * 3 + 1], tempCol, 1);
						}
						else
						{
							ChangeTile(POINT{ index.x + deltaDirection[i] * 2, index.y }, deltaCenterColTag[i * 3 + 1], randWall, 0);
							ChangeTile(POINT{ index.x + deltaDirection[i] * 2 + 1, index.y }, deltaCenterColTag[i * 3 + 1], randWall, 1);
						}
					}
					else
					{
						if (i == 0)
						{
							DrawWallCenter(POINT{ index.x + deltaDirection[i] * 2, index.y - 1 }, POINT{ index.x, index.y });
							DrawWallLeft(POINT{ index.x + deltaDirection[i] * 2, index.y - 1 }, POINT{ index.x, index.y });
							DrawWallLeftExtra(POINT{ index.x + deltaDirection[i] * 2, index.y - 1 }, POINT{ index.x, index.y });
							DrawWallTop(POINT{ index.x + deltaDirection[i] * 2, index.y - 1 }, POINT{ index.x, index.y });
							if (IS_TAG_IN_BIT(deltaOtherSideColTag[i * 3 + 2], tempTag))
							{
								DrawWallBottom(POINT{ index.x + deltaDirection[i] * 2, index.y - 1 }, POINT{ index.x, index.y });
								DrawWallBottomExtra(POINT{ index.x + deltaDirection[i] * 2, index.y - 1 }, POINT{ index.x, index.y });
							}
						}
						else
						{
							DrawWallCenter(POINT{ index.x, index.y - 1 }, POINT{ index.x + deltaDirection[i] * 2, index.y });
							DrawWallRight(POINT{ index.x, index.y - 1 }, POINT{ index.x + deltaDirection[i] * 2, index.y });
							DrawWallRightExtra(POINT{ index.x, index.y - 1 }, POINT{ index.x + deltaDirection[i] * 2, index.y });
							DrawWallTop(POINT{ index.x, index.y - 1 }, POINT{ index.x + deltaDirection[i] * 2, index.y });
							if (IS_TAG_IN_BIT(deltaOtherSideColTag[i * 3 + 2], tempTag))
							{
								DrawWallBottom(POINT{ index.x, index.y - 1 }, POINT{ index.x + deltaDirection[i] * 2, index.y });
								DrawWallBottomExtra(POINT{ index.x, index.y - 1 }, POINT{ index.x + deltaDirection[i] * 2, index.y });
							}
						}
					}
				}
			}
			else if (tag == deltaOtherTag[i * 7 + 1])
			{
				int tempCol = 0;
				ChangeTile(index, deltaSideColTag[i * 5], randWall, 0);
				ChangeTile(POINT{ index.x + 1, index.y }, deltaSideColTag[i * 5], randWall, 1);
				if (GetTileColumn(POINT{ index.x + deltaDirection[i] * 2, index.y + 1 }, tempCol))
				{
					ChangeTile(POINT{ index.x + deltaDirection[i] * 2, index.y }, deltaSideColTag[i * 5 + 1], tempCol, 0);
					ChangeTile(POINT{ index.x + deltaDirection[i] * 2 + 1, index.y }, deltaSideColTag[i * 5 + 1], tempCol, 1);
				}
				else
				{
					ChangeTile(POINT{ index.x + deltaDirection[i] * 2, index.y }, deltaSideColTag[i * 5 + 1], randWall, 0);
					ChangeTile(POINT{ index.x + deltaDirection[i] * 2 + 1, index.y }, deltaSideColTag[i * 5 + 1], randWall, 1);
				}
				ChangeTile(POINT{ index.x, index.y + 1 }, deltaSideColTag[i * 5 + 2], randWall, 0);
				ChangeTile(POINT{ index.x + 1, index.y + 1 }, deltaSideColTag[i * 5 + 2], randWall, 1);
				if (GetTileColumn(POINT{ index.x + deltaDirection[i] * -2, index.y - 1 }, tempCol))
				{
					ChangeTile(POINT{ index.x + deltaDirection[i] * -2, index.y + 1 }, deltaSideColTag[i * 5 + 3], tempCol, 0);
					ChangeTile(POINT{ index.x + deltaDirection[i] * -2 + 1, index.y + 1 }, deltaSideColTag[i * 5 + 3], tempCol, 1);
				}
				else
				{
					ChangeTile(POINT{ index.x + deltaDirection[i] * -2, index.y + 1 }, deltaSideColTag[i * 5 + 3], randWall, 0);
					ChangeTile(POINT{ index.x + deltaDirection[i] * -2 + 1, index.y + 1 }, deltaSideColTag[i * 5 + 3], randWall, 1);
				}
				ChangeTile(POINT{ index.x, index.y - 1 }, deltaSideColTag[i * 5 + 4], randWall, 0);
				ChangeTile(POINT{ index.x + 1, index.y - 1 }, deltaSideColTag[i * 5 + 4], randWall, 1);
			}
			else if (tag == deltaOtherTag[i * 7 + 2])
			{
				ChangeTile(index, deltaTag[i], randWall, 0);
				ChangeTile(POINT{ index.x + 1, index.y }, deltaTag[i], randWall, 1);
			}
			else if (IS_TAG_IN_BIT(deltaOtherTag[i * 7 + 3], tag))
			{
				int col = 0;
				if (GetTileColumn(POINT{ index.x, index.y - 1 }, col))
				{
					ChangeTile(index, deltaTag[i], col, 0);
					ChangeTile(POINT{ index.x + 1, index.y }, deltaTag[i], col, 1);
				}
				else
				{
					ChangeTile(index, deltaTag[i], randWall, 0);
					ChangeTile(POINT{ index.x + 1, index.y }, deltaTag[i], randWall, 1);
				}
			}
			else if (tag == (int)Tile::eTileTag::Dirt || tag == deltaTag[i] || IS_TAG_IN_BIT(deltaOtherTag[i * 7 + 4], tag) || IS_TAG_IN_BIT(deltaOtherTag[i * 7 + 5], tag) || (deltaOtherTag[i * 7 + 6] == tag))
			{
				int col = 0;
				if (GetTileColumn(POINT{ index.x, index.y - 1 }, col))
				{
					ChangeTile(index, deltaTag[i], col, 0);
					ChangeTile(POINT{ index.x + 1, index.y }, deltaTag[i], col, 1);
				}
				else
				{
					ChangeTile(index, deltaTag[i], randWall, 0);
					ChangeTile(POINT{ index.x + 1, index.y }, deltaTag[i], randWall, 1);
				}
			}
		}
	}
}

void MapToolScene::DrawWallBottomExtra(const POINT& lt, const POINT& rb, int col)
{
	POINT index = { lt.x, lt.y };
	int tag;

	static const int deltaTag[] = {
		(int)Tile::eTileTag::BottomExtra_L,
		(int)Tile::eTileTag::BottomExtra_R,
	};

	static const int deltaOtherTag[] = {
		(int)Tile::eTileTag::Top_L,
		(int)Tile::eTileTag::LeftExtra_B,
		(int)Tile::eTileTag::RightExtra_T,

		(int)Tile::eTileTag::Top_R,
		(int)Tile::eTileTag::RightExtra_B,
		(int)Tile::eTileTag::LeftExtra_T,
	};

	static const int deltaOffset[] = {
		0, 3,
		2, 3,
	};

	static const int deltaDirection[] = {
		-1, 1
	};

	static const int deltaColTag[] = {
		(int)Tile::eTileTag::Top_L | (int)Tile::eTileTag::BottomExtra_L,
		(int)Tile::eTileTag::Top_R | (int)Tile::eTileTag::BottomExtra_R,
	};

	size_t size = ARRAYSIZE(deltaTag);

	for (size_t i = 0; i < size; ++i)
	{
		index = POINT{ lt.x + deltaOffset[i * 2], lt.y + deltaOffset[i * 2 + 1] };

		if (GetTileTag(index, tag))
		{
			int randWall = 0;
			if (col < 0)
			{
				if (!GetTileColumn(POINT{ index.x, index.y - 1 }, randWall))
				{
					randWall = RANDOM->GetValue(0, Tile::WALL_MAX_COL - 1);
				}
			}
			else { randWall = col; }

			if (tag == deltaOtherTag[i * 3])
			{
				ChangeTile(index, deltaColTag[i], randWall, 0);
				ChangeTile(POINT{ index.x + 1, index.y }, deltaColTag[i], randWall, 1);
			}
			else if (tag == (int)Tile::eTileTag::Dirt || tag == deltaTag[i] || tag == deltaOtherTag[i * 3 + 1] || tag == deltaOtherTag[i * 3 + 2])
			{
				ChangeTile(index, deltaTag[i], randWall, 0);
				ChangeTile(POINT{ index.x + 1, index.y }, deltaTag[i], randWall, 1);
			}
		}
	}
}

bool MapToolScene::GetTileTag(const POINT& index, int& result)
{
	if (index.x < 0 || index.y < 0 || index.x >= mMapWidth || index.y >= mMapHeight)
	{
		return false;
	}
	result = mVecTile[index.x + index.y * mMapWidth]->GetCurrentTag();
	return true;
}

bool MapToolScene::GetTileColumn(const POINT& index, int& result)
{
	if (index.x < 0 || index.y < 0 || index.x >= mMapWidth || index.y >= mMapHeight)
	{
		return false;
	}
	result = mVecTile[index.x + index.y * mMapWidth]->GetCurrentColumn();
	return true;
}

void MapToolScene::ChangeTile(const POINT& index, int tag, int col, int offset)
{
	if (index.x < 0 || index.y < 0 || index.x >= mMapWidth || index.y >= mMapHeight) { return; }

	mVecTile[index.x + index.y * mMapWidth]->SetTileSprite(tag, col, offset);
}

void MapToolScene::SetIsLineActive(bool set)
{
	mbIsDrawTerrain = set;
	mpLineLT->SetIsRender(set);
	mpLineLB->SetIsRender(set);
	mpLineRT->SetIsRender(set);
	mpLineRB->SetIsRender(set);
}

void MapToolScene::ChangeToNone()
{
	SetIsLineActive(true);
	mpCurObject->ChangeSprite(eBitmapTag::NONE);
	mpCurObject->SetMaxFrame(1, 1);
}

void MapToolScene::ChangeToBarrack()
{
	mCurBuildingTag = eBuildingTag::Barrack;
	SetIsLineActive(false);
	mpCurObject->ChangeSprite(eBitmapTag::BUILDING_BARRACK);
	mpCurObject->SetMaxFrame(SpriteData::BARRACK_FRAME_X, SpriteData::BARRACK_FRAME_Y);
	mpCurObject->ChangeColor(EFFECT_COLOR_RED);
	mArrCurObjectColor = EFFECT_COLOR_RED;
}

void MapToolScene::ChangeToFactory()
{
	mCurBuildingTag = eBuildingTag::Factory;
	SetIsLineActive(false);
	mpCurObject->ChangeSprite(eBitmapTag::BUILDING_FACTORY);
	mpCurObject->SetMaxFrame(SpriteData::FACTORY_FRAME_X, SpriteData::FACTORY_FRAME_Y);
	mpCurObject->ChangeColor(EFFECT_COLOR_RED);
	mArrCurObjectColor = EFFECT_COLOR_RED;
}

void MapToolScene::ChangeToGroup1StartLocation()
{
	mCurBuildingTag = eBuildingTag::Group1StartLocation;
	SetIsLineActive(false);
	mpCurObject->ChangeSprite(eBitmapTag::BUILDING_START);
	mpCurObject->SetMaxFrame(SpriteData::START_FRAME_X, SpriteData::START_FRAME_Y);
	mpCurObject->ChangeColor(EFFECT_COLOR_RED);
	mArrCurObjectColor = EFFECT_COLOR_RED;
}

void MapToolScene::ChangeToGroup2StartLocation()
{
	mCurBuildingTag = eBuildingTag::Group2StartLocation;
	SetIsLineActive(false);
	mpCurObject->ChangeSprite(eBitmapTag::BUILDING_START);
	mpCurObject->SetMaxFrame(SpriteData::START_FRAME_X, SpriteData::START_FRAME_Y);
	mpCurObject->ChangeColor(EFFECT_COLOR_BLUE);
	mArrCurObjectColor = EFFECT_COLOR_BLUE;
}

void MapToolScene::SaveMap(LPCWSTR path)
{
	static const int ARR_GROUND_TILE[] = {
		0, 1,
		6, 11,
		24, 31,
	};
	static const int arrSize = 6;
	ofstream fout(path);

	fout << mMapWidth << endl;
	fout << mMapHeight << endl;

	Tile* pTile = nullptr;

	for (int y = 0; y < mMapHeight; ++y)
	{
		for (int x = 0; x < mMapWidth; ++x)
		{
			pTile = mVecTile[x + y * mMapWidth];
			fout << ((int)pTile->GetPosition().x / Tile::TILE_SIZE) << ' ' <<
				((int)pTile->GetPosition().y / Tile::TILE_SIZE) << ' ' <<
				pTile->GetCurrentRow() << ' ' << pTile->GetCurrentColumn();
			bool bIsObstacle = true;
			for (int i = 0; i < arrSize; ++i)
			{
				if (pTile->GetCurrentRow() == ARR_GROUND_TILE[i])
				{
					bIsObstacle = false;
					break;
				}
			}
			if (bIsObstacle)
			{
				fout << ' ' << true << endl;
			}
			else
			{
				fout << ' ' << false << endl;
			}
		}
	}

	for (int y = 0; y < mMapHeight; ++y)
	{
		for (int x = 0; x < mMapWidth; ++x)
		{
			if (mVecBuildingTag[x + y * mMapWidth] != eBuildingTag::None)
			{
				fout << x << ' ' << y << ' ' << (int)mVecBuildingTag[x + y * mMapWidth] << endl;
			}
		}
	}

	fout.close();
}
