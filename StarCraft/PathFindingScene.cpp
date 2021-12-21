#include "stdafx.h"
#include "PathFindingScene.h"
#include "RenderManager.h"
#include "PhysicsManager.h"
#include "Unit.h"
#include "PathFinder.h"
#include "TileManager.h"
#include "UIManager.h"
#include "UserManager.h"
#include "UnitManager.h"

void PathFindingScene::Enter()
{
	UNIT->Init(this);
	USER->Init(eTeamTag::RED_TEAM);

	UI->Init();
	TILE->Init();
	TILE->LoadTileMap(this, TEXT("MapData/Test.txt"));

	RANDOM->SetSeed(time(nullptr));

	for (int i = 1; i <= 12; ++i)
	{
		UNIT->CreateUnit(eTeamTag::RED_TEAM, eUnitTag::Marrine, 25, 25 * i, i);
	}

	for (int i = 1; i <= 6; ++i)
	{
		UNIT->CreateUnit(eTeamTag::BLUE_TEAM, eUnitTag::Marrine, 500, 25 * i, 12 + i);
	}
}

void PathFindingScene::Exit()
{
	TILE->Release();
	UI->Release();
	USER->Release();
	UNIT->Release();
}

void PathFindingScene::Update()
{
	UNIT->Update();
	USER->Update();

	if (INPUT->IsStayKeyDown(VK_LSHIFT))
	{
		if (INPUT->IsStayKeyDown(VK_LEFT)) { CAMERA->AddPosition(Vector2::Right() * CAMERA_FAST_MOVING_SPEED * DELTA_TIME); }
		if (INPUT->IsStayKeyDown(VK_RIGHT)) { CAMERA->AddPosition(Vector2::Left() * CAMERA_FAST_MOVING_SPEED * DELTA_TIME); }
		if (INPUT->IsStayKeyDown(VK_UP)) { CAMERA->AddPosition(Vector2::Down() * CAMERA_FAST_MOVING_SPEED * DELTA_TIME); }
		if (INPUT->IsStayKeyDown(VK_DOWN)) { CAMERA->AddPosition(Vector2::Up() * CAMERA_FAST_MOVING_SPEED * DELTA_TIME); }
	}
	else
	{
		if (INPUT->IsStayKeyDown(VK_LEFT)) { CAMERA->AddPosition(Vector2::Right() * CAMERA_MOVING_SPEED * DELTA_TIME); }
		if (INPUT->IsStayKeyDown(VK_RIGHT)) { CAMERA->AddPosition(Vector2::Left() * CAMERA_MOVING_SPEED * DELTA_TIME); }
		if (INPUT->IsStayKeyDown(VK_UP)) { CAMERA->AddPosition(Vector2::Down() * CAMERA_MOVING_SPEED * DELTA_TIME); }
		if (INPUT->IsStayKeyDown(VK_DOWN)) { CAMERA->AddPosition(Vector2::Up() * CAMERA_MOVING_SPEED * DELTA_TIME); }
	}

	// GameRoot���� Update�� Render���̿� �� ��
	UI->Update();
}
