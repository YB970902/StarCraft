#include "stdafx.h"
#include "PathFindingScene.h"
#include "RenderManager.h"
#include "PhysicsManager.h"
#include "Unit.h"
#include "PathFinder.h"
#include "TileManager.h"
#include "UIManager.h"

void PathFindingScene::Enter()
{
	TILE->Init();
	TILE->LoadTileMap(this, TEXT("MapData/Test.txt"));
	UIManager::GetInstance()->Init();

	RANDOM->SetSeed(time(nullptr));

	mpFirstUnit = static_cast<Unit*>(AddGameObject(new Unit(eTeamTag::RED_TEAM)));
	mpFirstUnit->SetPosition(25, 25);

	mpSecondUnit = new Unit(eTeamTag::RED_TEAM);
	AddGameObject(mpSecondUnit);
	mpSecondUnit->SetPosition(55, 25);

	mpCurUnit = mpFirstUnit;
}

void PathFindingScene::Exit()
{
	TILE->Release();
	UIManager::GetInstance()->Release();
}

void PathFindingScene::Update()
{
	if (INPUT->IsOnceKeyDown(VK_RBUTTON))
	{
		mpCurUnit->FindPath(INPUT->GetMousePosition());
	}

	Unit* pUnit = nullptr;
	if (PHYSICS->GetUnit(eTeamTag::RED_TEAM, INPUT->GetMousePosition(), &pUnit))
	{
		UIManager::GetInstance()->ChangeCursorState(eCursorState::OnGreen);
		if (INPUT->IsOnceKeyDown('E'))
		{
			pUnit->ChangeCircleColor(EFFECT_COLOR_RED);
		}
		if (INPUT->IsOnceKeyDown('R'))
		{
			pUnit->ChangeCircleColor(EFFECT_COLOR_GREEN);
		}
	}
	else
	{
		UIManager::GetInstance()->ChangeCursorState(eCursorState::Idle);
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

	UIManager::GetInstance()->Update();
}
