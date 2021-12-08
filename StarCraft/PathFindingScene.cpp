#include "stdafx.h"
#include "PathFindingScene.h"
#include "RenderManager.h"
#include "Unit.h"
#include "DetailMap.h"
#include "Unit.h"
#include "PathFinder.h"
#include "TileManager.h"
#include <chrono>
#include <time.h>
#include <conio.h>

list<TileCoord> nodeList;
int g_count = 0;
int g_maxCount = 50;
int g_index = 0;
bool g_isCounting = false;

void PathFindingScene::Enter()
{
	mpDetailMap = new DetailMap();
	mMapWidth = 50;
	mMapHeight = 50;
	mpDetailMap->Init(mMapWidth, mMapHeight);
	srand(13);
	int startX = 0;
	int startY = 0;
	int endX = mMapWidth - 1;
	int endY = mMapHeight - 1;
	for (int i = 0; i < mMapWidth; ++i)
	{
		for (int j = 0; j < mMapHeight; ++j)
		{
			if ((i == startX && j == startY) || (i == endX && j == endY)) continue;
			if (rand() % 10 == 0) mpDetailMap->SetAt(i, j);
		}
	}
	mpDetailMap->UpdateGizmo();

	mpPathFinder = new PathFinder();
	mpPathFinder->Init(mpDetailMap);

	if (mpPathFinder->FindPath(startX, startY, endX, endY, nodeList))
	{
		nodeList.pop_front();
		nodeList.pop_back();
		g_isCounting = true;
	}
	mpDetailMap->ChangeGizmoColor(startX, startY, D2D1::ColorF::Orange);
	mpDetailMap->ChangeGizmoColor(endX, endY, D2D1::ColorF::Black);
}

void PathFindingScene::Exit()
{
	SAFE_RELEASE(mpDetailMap);
	SAFE_RELEASE(mpPathFinder);
}

void PathFindingScene::Update()
{
	if (g_isCounting)
	{
		++g_count;
		if (g_count >= g_maxCount)
		{
			g_count = 0;
			if (nodeList.empty())
			{
				g_isCounting = false;
			}
			else
			{
				mpDetailMap->ChangeGizmoColor(nodeList.front().GetX(), nodeList.front().GetY(), D2D1::ColorF::Blue);
				nodeList.pop_front();
			}
		}
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
