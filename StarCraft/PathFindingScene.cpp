#include "stdafx.h"
#include "PathFindingScene.h"
#include "RenderManager.h"
#include "PhysicsManager.h"
#include "Unit.h"
#include "PathFinder.h"
#include "TileManager.h"
#include "UserManager.h"
#include "UnitManager.h"
#include "ParticleManager.h"

void PathFindingScene::Enter()
{
	SOUND->StopAll();
	SOUND->Play(eSoundTag::TerranTheme);
	PARTICLE->Init(this);

	USER->SetTeamTag(eTeamTag::RED_TEAM);
	USER->Init();
	UNIT->Init(this);

	TILE->Init();
	TILE->LoadTileMap(this, TEXT("MapData/Test.txt"));

	RANDOM->SetSeed(time(nullptr));

	for (int i = 1; i <= 12; ++i)
	{
		UNIT->CommandCreateUnit(eTeamTag::RED_TEAM, eUnitTag::Goliath, 200, 200);
		UNIT->CommandCreateUnit(eTeamTag::RED_TEAM, eUnitTag::Marine, 200, 200);
	}

	for (int i = 1; i <= 12; ++i)
	{
		UNIT->CommandCreateUnit(eTeamTag::BLUE_TEAM, eUnitTag::Marine, 800, 200);
		UNIT->CommandCreateUnit(eTeamTag::BLUE_TEAM, eUnitTag::Goliath, 800, 200);
	}
}

void PathFindingScene::Exit()
{
	SOUND->StopAll();
	PARTICLE->Release();
	USER->Release();
	UNIT->Release();

	TILE->Release();
}

void PathFindingScene::Update()
{
	if (INPUT->IsOnceKeyDown(VK_BACK))
	{
		SCENE->ChangeScene(eSceneTag::MainScene);
		return;
	}

	UNIT->Update();
	USER->Update();
	PARTICLE->Update();

	if (INPUT->IsOnceKeyDown('1'))
	{
		UNIT->CommandCreateUnit(eTeamTag::RED_TEAM, eUnitTag::Goliath, 400, 200);
		UNIT->CommandCreateUnit(eTeamTag::RED_TEAM, eUnitTag::Marine, 400, 200);
	}

	if (INPUT->IsOnceKeyDown('2'))
	{
		UNIT->CommandCreateUnit(eTeamTag::BLUE_TEAM, eUnitTag::Goliath, 500, 200);
		UNIT->CommandCreateUnit(eTeamTag::BLUE_TEAM, eUnitTag::Marine, 500, 200);
	}

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
}
