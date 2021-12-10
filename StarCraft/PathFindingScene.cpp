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

	mpUnit = new Unit();
	AddGameObject(mpUnit);
	mpUnit->SetPosition(5, 5);
}

void PathFindingScene::Exit()
{
	TILE->Release();
}

void PathFindingScene::Update()
{
	if (INPUT->IsOnceKeyDown(VK_RBUTTON))
	{
		mpUnit->FindPath(INPUT->GetMousePosition());
	}
	if (INPUT->IsStayKeyDown(VK_LSHIFT))
	{
		if (INPUT->IsStayKeyDown('A')) { RENDER->AddCameraPosition(Vector2::Right() * CAMERA_FAST_MOVING_SPEED * DELTA_TIME); }
		if (INPUT->IsStayKeyDown('D')) { RENDER->AddCameraPosition(Vector2::Left() * CAMERA_FAST_MOVING_SPEED * DELTA_TIME); }
		if (INPUT->IsStayKeyDown('W')) { RENDER->AddCameraPosition(Vector2::Down() * CAMERA_FAST_MOVING_SPEED * DELTA_TIME); }
		if (INPUT->IsStayKeyDown('S')) { RENDER->AddCameraPosition(Vector2::Up() * CAMERA_FAST_MOVING_SPEED * DELTA_TIME); }
	}
	else
	{
		if (INPUT->IsStayKeyDown('A')) { RENDER->AddCameraPosition(Vector2::Right() * CAMERA_MOVING_SPEED * DELTA_TIME); }
		if (INPUT->IsStayKeyDown('D')) { RENDER->AddCameraPosition(Vector2::Left() * CAMERA_MOVING_SPEED * DELTA_TIME); }
		if (INPUT->IsStayKeyDown('W')) { RENDER->AddCameraPosition(Vector2::Down() * CAMERA_MOVING_SPEED * DELTA_TIME); }
		if (INPUT->IsStayKeyDown('S')) { RENDER->AddCameraPosition(Vector2::Up() * CAMERA_MOVING_SPEED * DELTA_TIME); }
	}
}
