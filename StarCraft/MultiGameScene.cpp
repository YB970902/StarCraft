#include "stdafx.h"
#include "MultiGameScene.h"
#include "RenderManager.h"
#include "PhysicsManager.h"
#include "Unit.h"
#include "PathFinder.h"
#include "TileManager.h"
#include "UIManager.h"
#include "UserManager.h"
#include "UnitManager.h"
#include "ParticleManager.h"
#include "NetworkManager.h"

const Fix CAMERA_MOVING_SPEED = 500;
const Fix CAMERA_FAST_MOVING_SPEED = 1000;

void MultiGameScene::Enter()
{
	NET->InitGame();

	SOUND->StopAll();
	SOUND->Play(eSoundTag::TerranTheme);
	PARTICLE->Init(this);

	UI->Init();
	USER->Init();
	// 이거 메시지 받으면 바꿀 수 있도록 수정
	//USER->Init(eTeamTag::RED_TEAM);
	UNIT->Init(this);

	TILE->Init();
	TILE->LoadTileMap(this, TEXT("MapData/Test.txt"));

	// 이거 메시지 받으면 바꿀 수 있도록 수정
	//RANDOM->SetSeed(0);

	for (int i = 0; i < 12; ++i)
	{
		UNIT->CommandCreateUnit(eTeamTag::RED_TEAM, eUnitTag::Goliath, 200, 200);
		UNIT->CommandCreateUnit(eTeamTag::RED_TEAM, eUnitTag::Marine, 200, 200);
	}

	for (int i = 0; i < 12; ++i)
	{
		UNIT->CommandCreateUnit(eTeamTag::BLUE_TEAM, eUnitTag::Goliath, 800, 200);
		UNIT->CommandCreateUnit(eTeamTag::BLUE_TEAM, eUnitTag::Marine, 800, 200);
	}
}

void MultiGameScene::Exit()
{
	NET->ReleaseGame();

	SOUND->StopAll();

	PARTICLE->Release();
	UI->Release();
	USER->Release();
	UNIT->Release();
	TILE->Release();
}

void MultiGameScene::Update()
{
	if (INPUT->IsOnceKeyDown(VK_ESCAPE))
	{
		SCENE->ChangeScene(eSceneTag::MatchingScene);
		return;
	}

	NET->Update();
	mbIsCanUpdate = NET->UpdateGame();

	if (mbIsCanUpdate)
	{
		UNIT->Update();
		USER->Update();
		PARTICLE->Update();
	}

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

	// GameRoot에서 Update와 Render사이에 둘 것
	UI->Update();
}

void MultiGameScene::ReceiveMessage(Message* pMsg)
{
}
