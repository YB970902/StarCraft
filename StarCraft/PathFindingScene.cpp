#include "stdafx.h"
#include "PathFindingScene.h"
#include "RenderManager.h"
#include "Unit.h"
#include "PathFinder.h"
#include "TileManager.h"

void PathFindingScene::Enter()
{
	TILE->Init();
	TILE->LoadTileMap();

	mpFirstUnit = new Unit();
	AddGameObject(mpFirstUnit);
	mpFirstUnit->SetPosition(5, 5);

	mpSecondUnit = new Unit();
	AddGameObject(mpSecondUnit);
	mpSecondUnit->SetPosition(55, 5);

	mpCurUnit = mpFirstUnit;
}

void PathFindingScene::Exit()
{
	TILE->Release();
}

void PathFindingScene::Update()
{
	if (INPUT->IsOnceKeyDown(VK_RBUTTON))
	{
		mpCurUnit->FindPath(INPUT->GetMousePosition());
	}

	if (INPUT->IsOnceKeyDown('1'))
	{
		mpCurUnit = mpFirstUnit;
	}
	if (INPUT->IsOnceKeyDown('2'))
	{
		mpCurUnit = mpSecondUnit;
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
