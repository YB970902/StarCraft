#include "stdafx.h"
#include "PhysicsManager.h"
#include "GameObject.h"
#include "Unit.h"
#include "ColliderComponent.h"
#include "RendererComponent.h"
#include "TransformComponent.h"

void PhysicsManager::Init()
{
}

void PhysicsManager::Release()
{
}

void PhysicsManager::InitLayerSize(int width, int height)
{
	div_t dv = div(width, GRID_SIZE);
	if (dv.rem) { dv.quot++; }
	mGridWidth = dv.quot;
	dv = div(height, GRID_SIZE);
	if (dv.rem) { dv.quot++; }
	mGridHeight = dv.quot;

	mVecRedTeamObjectGrid.resize(mGridWidth * mGridHeight);
	mVecBlueTeamObjectGrid.resize(mGridWidth * mGridHeight);
}

void PhysicsManager::AddCollider(ColliderComponent* pCollider, const Vector2& pos, const Vector2& size)
{
	vector<vector<ColliderComponent*>>* pCurGrid = nullptr;

	switch (pCollider->GetTeamTag())
	{
	case eTeamTag::RED_TEAM:
		pCurGrid = &mVecRedTeamObjectGrid;
		break;
	case eTeamTag::BLUE_TEAM:
		pCurGrid = &mVecBlueTeamObjectGrid;
		break;
	default:
		return;
	}

	POINT leftTop = POINT{ (int)(pos.x - size.x) / GRID_SIZE, (int)(pos.y - size.y) / GRID_SIZE };
	POINT rightBottom = POINT{ (int)(pos.x + size.x) / GRID_SIZE, (int)(pos.y + size.y) / GRID_SIZE };

	AddColliderByIndex(pCollider, leftTop, rightBottom);
}

void PhysicsManager::RemoveCollider(ColliderComponent* pCollider, const Vector2& pos, const Vector2& size)
{
	vector<vector<ColliderComponent*>>* pCurGrid = nullptr;

	switch (pCollider->GetTeamTag())
	{
	case eTeamTag::RED_TEAM:
		pCurGrid = &mVecRedTeamObjectGrid;
		break;
	case eTeamTag::BLUE_TEAM:
		pCurGrid = &mVecBlueTeamObjectGrid;
		break;
	default:
		return;
	}

	POINT leftTop = POINT{ (int)(pos.x - size.x) / GRID_SIZE, (int)(pos.y - size.y) / GRID_SIZE };
	POINT rightBottom = POINT{ (int)(pos.x + size.x) / GRID_SIZE, (int)(pos.y + size.y) / GRID_SIZE };

	RemoveColliderByIndex(pCollider, leftTop, rightBottom);
}

void PhysicsManager::ProcessObjectMove(ColliderComponent* pCollider, const Vector2& prevPos, const Vector2& size)
{	
	POINT prevLeftTop = POINT{ (int)(prevPos.x - size.x) / GRID_SIZE, (int)(prevPos.y - size.y) / GRID_SIZE };
	POINT prevRightBottom = POINT{ (int)(prevPos.x + size.x) / GRID_SIZE, (int)(prevPos.y + size.y) / GRID_SIZE };

	//if (prevLeftTop.x < 0) { prevLeftTop.x = 0; }
	//if (prevLeftTop.y < 0) { prevLeftTop.y = 0; }
	//
	//if (prevRightBottom.x >= mGridWidth) { prevRightBottom.x = mGridWidth - 1; }
	//if (prevRightBottom.y >= mGridHeight) { prevRightBottom.y = mGridHeight - 1; }

	Vector2 pos = pCollider->GetColliderPosition();
	POINT leftTop = POINT{ (int)(pos.x - size.x) / GRID_SIZE, (int)(pos.y - size.y) / GRID_SIZE };
	POINT rightBottom = POINT{ (int)(pos.x + size.x) / GRID_SIZE, (int)(pos.y + size.y) / GRID_SIZE };

	//if (leftTop.x < 0) { leftTop.x = 0; }
	//if (leftTop.y < 0) { leftTop.y = 0; }
	//
	//if (rightBottom.x >= mGridWidth) { rightBottom.x = mGridWidth - 1; }
	//if (rightBottom.y >= mGridHeight) { rightBottom.y = mGridHeight - 1; }

	if (prevLeftTop.x == leftTop.x && prevLeftTop.y == leftTop.y &&
		prevRightBottom.x == rightBottom.x && prevRightBottom.y == rightBottom.y)
	{
		return;
	}

	RemoveCollider(pCollider, prevPos, size);
	AddCollider(pCollider, pos, size);
	//
	//// 너무 많이 이동한 경우
	//if (prevLeftTop.x > rightBottom.x || prevRightBottom.x < leftTop.x ||
	//	prevLeftTop.y > rightBottom.y || prevRightBottom.y < leftTop.y)
	//{
	//	RemoveColliderByIndex(pCollider, prevLeftTop, prevRightBottom);
	//	AddColliderByIndex(pCollider, leftTop, rightBottom);
	//
	//	return;
	//}
	//
	//// X축 이동 처리
	//
	//if (leftTop.x > prevLeftTop.x)
	//{
	//	RemoveColliderByIndex(pCollider, POINT{ prevLeftTop.x, prevLeftTop.y }, POINT{ leftTop.x - 1, prevRightBottom.y });
	//}
	//else if (leftTop.x < prevLeftTop.x)
	//{
	//	AddColliderByIndex(pCollider, POINT{ leftTop.x, leftTop.y }, POINT{ prevLeftTop.x - 1, rightBottom.y });
	//}
	//
	//if (rightBottom.x > prevRightBottom.x)
	//{
	//	AddColliderByIndex(pCollider, POINT{ prevRightBottom.x + 1, leftTop.y }, POINT{ rightBottom.x, rightBottom.y });
	//}
	//else if (rightBottom.x < prevRightBottom.x)
	//{
	//	RemoveColliderByIndex(pCollider, POINT{ rightBottom.x + 1, prevLeftTop.x }, POINT{ prevRightBottom.x, prevRightBottom.y });
	//}
	//
	//// Y축 이동 처리
	//
	//if (leftTop.y > prevLeftTop.y)
	//{
	//	RemoveColliderByIndex(pCollider, POINT{ prevLeftTop.x, prevLeftTop.y }, POINT{ prevRightBottom.x, leftTop.y - 1 });
	//}
	//else if (leftTop.y < prevLeftTop.y)
	//{
	//	AddColliderByIndex(pCollider, POINT{ leftTop.x, leftTop.y }, POINT{ rightBottom.x, prevLeftTop.y - 1 });
	//}
	//
	//if (rightBottom.y > prevRightBottom.y)
	//{
	//	AddColliderByIndex(pCollider, POINT{ leftTop.x, prevRightBottom.y + 1 }, POINT{ rightBottom.x, rightBottom.y });
	//}
	//else if (rightBottom.y < prevRightBottom.y)
	//{
	//	RemoveColliderByIndex(pCollider, POINT{ prevLeftTop.x, rightBottom.y + 1 }, POINT{ prevRightBottom.x, prevRightBottom.y });
	//}
}

bool PhysicsManager::GetUnit(eTeamTag teamTag, const POINT& pos, Unit** ppUnit)
{
	POINT index = { pos.x / GRID_SIZE, pos.y / GRID_SIZE };

	vector<ColliderComponent*>* pVecUnit = nullptr;

	switch (teamTag)
	{
	case eTeamTag::RED_TEAM:
		pVecUnit = &mVecRedTeamObjectGrid[index.x + index.y * mGridWidth];
		break;
	case eTeamTag::BLUE_TEAM:
		pVecUnit = &mVecRedTeamObjectGrid[index.x + index.y * mGridWidth];
		break;
	default:
		return false;
	}

	size_t size = pVecUnit->size();
	for (size_t i = 0; i < size; ++i)
	{
		if ((*pVecUnit)[i]->IsCollided(pos))
		{
			(*ppUnit) = (*pVecUnit)[i]->GetUnit();
			return true;
		}
	}

	return false;
}

void PhysicsManager::AddColliderByIndex(ColliderComponent* pCollider, const POINT& leftTop, const POINT& rightBottom)
{
	if (leftTop.x < 0) { const_cast<POINT&>(leftTop).x = 0; }
	if (leftTop.y < 0) { const_cast<POINT&>(leftTop).y = 0; }

	if (rightBottom.x >= mGridWidth) { const_cast<POINT&>(rightBottom).x = mGridWidth - 1; }
	if (rightBottom.y >= mGridHeight) { const_cast<POINT&>(rightBottom).y = mGridHeight - 1; }

	vector<vector<ColliderComponent*>>* pCurGrid = nullptr;

	switch (pCollider->GetTeamTag())
	{
	case eTeamTag::RED_TEAM:
		pCurGrid = &mVecRedTeamObjectGrid;
		break;
	case eTeamTag::BLUE_TEAM:
		pCurGrid = &mVecBlueTeamObjectGrid;
		break;
	default:
		return;
	}

	int width = rightBottom.x - leftTop.x + 1;
	int height = rightBottom.y - leftTop.y + 1;

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			(*pCurGrid)[(leftTop.x + x) + (leftTop.y + y) * mGridWidth].push_back(pCollider);
		}
	}
}

void PhysicsManager::RemoveColliderByIndex(ColliderComponent* pCollider, const POINT& leftTop, const POINT& rightBottom)
{
	if (leftTop.x < 0) { const_cast<POINT&>(leftTop).x = 0; }
	if (leftTop.y < 0) { const_cast<POINT&>(leftTop).y = 0; }

	if (rightBottom.x >= mGridWidth) { const_cast<POINT&>(rightBottom).x = mGridWidth - 1; }
	if (rightBottom.y >= mGridHeight) { const_cast<POINT&>(rightBottom).x = mGridHeight - 1; }

	vector<vector<ColliderComponent*>>* pCurGrid = nullptr;

	switch (pCollider->GetTeamTag())
	{
	case eTeamTag::RED_TEAM:
		pCurGrid = &mVecRedTeamObjectGrid;
		break;
	case eTeamTag::BLUE_TEAM:
		pCurGrid = &mVecBlueTeamObjectGrid;
		break;
	default:
		return;
	}

	int width = rightBottom.x - leftTop.x + 1;
	int height = rightBottom.y - leftTop.y + 1;

	vector<ColliderComponent*>* pVecUnit = nullptr;

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			pVecUnit = &(*pCurGrid)[(leftTop.x + x) + (leftTop.y + y) * mGridWidth];
			auto it = find(pVecUnit->begin(), pVecUnit->end(), pCollider);
			if (it != pVecUnit->end())
			{
				pVecUnit->erase(it);
			}
		}
	}
}
