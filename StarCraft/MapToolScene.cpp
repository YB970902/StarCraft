#include "stdafx.h"
#include "MapToolScene.h"
#include "RenderManager.h"
#include "RectGizmo.h"
#include "TextGizmo.h"
#include "Tile.h"

void MapToolScene::Enter()
{
	RANDOM->SetSeed(time_t(NULL));

	mpUIBackground = (RectGizmo*)RENDER->RenderRect(Vector2(0, 0), Vector2(UI_WIDTH / 2, CAMERA->GetSize().height - UI_WIDTH / 2), UI_WIDTH, D2D1::ColorF(0, 0, 0, 0.5f), Vector2(0, 0));

	mpBtnGroundTile = (RectGizmo*)RENDER->RenderRect(Vector2(GROUND_BTN_X, GROUND_BTN_Y), Vector2(TILE_BTN_WIDTH / 2, TILE_BTN_HEIGHT / 2), TILE_BTN_WIDTH / 2, D2D1::ColorF(1, 1, 1, 1));
	mpBtnWallTile = (RectGizmo*)RENDER->RenderRect(Vector2(WALL_BTN_X, WALL_BTN_Y), Vector2(TILE_BTN_WIDTH / 2, TILE_BTN_HEIGHT / 2), TILE_BTN_WIDTH / 2, D2D1::ColorF(1, 1, 1, 1));

	mpTextGround = (TextGizmo*)RENDER->RenderText(TEXT("땅 그리기"), Vector2(GROUND_BTN_X - TILE_BTN_WIDTH / 2, GROUND_BTN_Y - FONT_SIZE / 2), Vector2(TILE_BTN_WIDTH, TILE_BTN_HEIGHT), FONT_SIZE);
	mpTextWall = (TextGizmo*)RENDER->RenderText(TEXT("벽 그리기"), Vector2(WALL_BTN_X - TILE_BTN_WIDTH / 2, WALL_BTN_Y - FONT_SIZE / 2), Vector2(TILE_BTN_WIDTH, TILE_BTN_HEIGHT), FONT_SIZE);

	SetUIActive(mbIsOpenUI);

	mVecTile.resize(mMapWidth * mMapHeight);
	for (int x = 0; x < mMapWidth; ++x)
	{
		for (int y = 0; y < mMapHeight; ++y)
		{
			mVecTile[x + y * mMapWidth] = static_cast<Tile*>(AddGameObject(new Tile));
			mVecTile[x + y * mMapWidth]->SetPosition((Fix)(x * Tile::TILE_SIZE), (Fix)(y * Tile::TILE_SIZE));
			mVecTile[x + y * mMapWidth]->SetTileSprite(Tile::eTileTag::Dirt);
		}
	}
}

void MapToolScene::Exit()
{
}

void MapToolScene::Update()
{
	if (INPUT->IsOnceKeyDown(VK_TAB))
	{
		mbIsOpenUI = !mbIsOpenUI;
		SetUIActive(mbIsOpenUI);
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

void MapToolScene::SetUIActive(bool set)
{
	mpUIBackground->SetIsActive(set);

	mpBtnGroundTile->SetIsActive(set);
	mpBtnWallTile->SetIsActive(set);

	mpTextGround->SetIsActive(set);
	mpTextWall->SetIsActive(set);
}
