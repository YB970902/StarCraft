#include "stdafx.h"
#include "PhysicsManager.h"
#include "GameObject.h"
#include "Unit.h"
#include "ColliderComponent.h"
#include "RendererComponent.h"
#include "TransformComponent.h"
#include "UnitManager.h"

void PhysicsManager::Init()
{
}

void PhysicsManager::Release()
{
	mbIsRelease = true;

	mVecRedTeamObjectGrid.clear();
	mVecBlueTeamObjectGrid.clear();
}

void PhysicsManager::InitDefaultLayer()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	InitLayerSize(rc.right - rc.left, rc.bottom - rc.top);
}

void PhysicsManager::InitLayerSize(int width, int height)
{
	mbIsRelease = false;

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
	if (mbIsRelease) { return; }
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
	if (mbIsRelease) { return; }

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
	if (mbIsRelease) { return; }

	POINT prevLeftTop = POINT{ (int)(prevPos.x - size.x) / GRID_SIZE, (int)(prevPos.y - size.y) / GRID_SIZE };
	POINT prevRightBottom = POINT{ (int)(prevPos.x + size.x) / GRID_SIZE, (int)(prevPos.y + size.y) / GRID_SIZE };

	Vector2 pos = pCollider->GetUnit()->GetPosition();
	POINT leftTop = POINT{ (int)(pos.x - size.x) / GRID_SIZE, (int)(pos.y - size.y) / GRID_SIZE };
	POINT rightBottom = POINT{ (int)(pos.x + size.x) / GRID_SIZE, (int)(pos.y + size.y) / GRID_SIZE };

	if (prevLeftTop.x == leftTop.x && prevLeftTop.y == leftTop.y &&
		prevRightBottom.x == rightBottom.x && prevRightBottom.y == rightBottom.y)
	{
		return;
	}

	RemoveCollider(pCollider, prevPos, size);
	AddCollider(pCollider, pos, size);
}

bool PhysicsManager::GetCollider(const POINT& pos, GameObject** ppObject)
{
	POINT index = { pos.x / GRID_SIZE, pos.y / GRID_SIZE };

	vector<ColliderComponent*>* pVecUnit = nullptr;

	pVecUnit = &mVecRedTeamObjectGrid[index.x + index.y * mGridWidth];
	size_t size = pVecUnit->size();
	for (size_t i = 0; i < size; ++i)
	{
		if ((*pVecUnit)[i]->IsCollided(pos))
		{
			(*ppObject) = (*pVecUnit)[i]->GetGameObject();
			return true;
		}
	}

	pVecUnit = &mVecBlueTeamObjectGrid[index.x + index.y * mGridWidth];
	size = pVecUnit->size();
	for (size_t i = 0; i < size; ++i)
	{
		if ((*pVecUnit)[i]->IsCollided(pos))
		{
			(*ppObject) = (*pVecUnit)[i]->GetGameObject();
			return true;
		}
	}

	return false;
}

bool PhysicsManager::GetUnit(eTeamTag teamTag, const POINT& pos, UnitID* pUnitID)
{
	POINT index = { pos.x / GRID_SIZE, pos.y / GRID_SIZE };

	vector<ColliderComponent*>* pVecUnit = nullptr;

	switch (teamTag)
	{
	case eTeamTag::RED_TEAM:
		pVecUnit = &mVecRedTeamObjectGrid[index.x + index.y * mGridWidth];
		break;
	case eTeamTag::BLUE_TEAM:
		pVecUnit = &mVecBlueTeamObjectGrid[index.x + index.y * mGridWidth];
		break;
	default:
		return false;
	}

	size_t size = pVecUnit->size();
	for (size_t i = 0; i < size; ++i)
	{
		if ((*pVecUnit)[i]->IsCollided(pos))
		{
			(*pUnitID) = (*pVecUnit)[i]->GetUnit()->GetUnitID();
			return true;
		}
	}

	return false;
}

bool PhysicsManager::GetDragUnit(eTeamTag teamTag, const POINT& startPos, const POINT& endPos, vector<UnitID>& outVecUnitID, int maxSize)
{
	RECT rc{
		min(startPos.x, endPos.x),
		min(startPos.y, endPos.y),
		max(startPos.x, endPos.x),
		max(startPos.y, endPos.y)
	};

	POINT leftTop = { rc.left / GRID_SIZE, rc.top / GRID_SIZE };
	POINT rightBottom = { rc.right / GRID_SIZE, rc.bottom / GRID_SIZE };
	int width = rightBottom.x - leftTop.x + 1;
	int height = rightBottom.y - leftTop.y + 1;

	vector<vector<ColliderComponent*>>* pCurGrid = nullptr;
	size_t size = 0;

	int curSize = 0;

	switch (teamTag)
	{
	case eTeamTag::RED_TEAM:
		pCurGrid = &mVecRedTeamObjectGrid;
		break;
	case eTeamTag::BLUE_TEAM:
		pCurGrid = &mVecBlueTeamObjectGrid;
		break;
	default:
		return false;
	}

	vector<ColliderComponent*>* pVecUnit = nullptr;

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			pVecUnit = &(*pCurGrid)[(leftTop.x + x) + (leftTop.y + y) * mGridWidth];
			size = pVecUnit->size();
			for (size_t i = 0; i < size; ++i)
			{
				if ((*pVecUnit)[i]->IsCollided(rc) && find(outVecUnitID.begin(), outVecUnitID.end(), (*pVecUnit)[i]->GetUnit()->GetUnitID()) == outVecUnitID.end())
				{
					++curSize;
					outVecUnitID.push_back((*pVecUnit)[i]->GetUnit()->GetUnitID());
					if (curSize == maxSize) { return true; }
				}
			}
		}
	}

	if (curSize == 0) { return false; }

	return true;
}

bool PhysicsManager::GetNearEnemyUnit(eTeamTag teamTag, const Vector2& pos, int range, UnitID* pUnitID)
{
	(*pUnitID) = UNIT_ID_NONE;

	vector<vector<ColliderComponent*>>* pCurGrid = nullptr;

	switch (teamTag)
	{
	case eTeamTag::RED_TEAM:
		pCurGrid = &mVecBlueTeamObjectGrid;
		break;
	case eTeamTag::BLUE_TEAM:
		pCurGrid = &mVecRedTeamObjectGrid;
		break;
	default:
		return false;
	}

	int halfRange = range / 2;

	RECT rc = {
		(int)pos.x - halfRange,
		(int)pos.y - halfRange,
		(int)pos.x + halfRange,
		(int)pos.y + halfRange
	};

	POINT leftTop = { rc.left / GRID_SIZE, rc.top / GRID_SIZE };
	POINT rightBottom = { rc.right / GRID_SIZE, rc.bottom / GRID_SIZE };

	if (leftTop.x < 0) { leftTop.x = 0; }
	if (leftTop.y < 0) { leftTop.y = 0; }

	if (rightBottom.x >= mGridWidth) { rightBottom.x = mGridWidth - 1; }
	if (rightBottom.y >= mGridHeight) { rightBottom.y = mGridHeight - 1; }

	int width = rightBottom.x - leftTop.x + 1;
	int height = rightBottom.y - leftTop.y + 1;

	int minDist = range;
	Unit* pMinUnit = nullptr;

	vector<ColliderComponent*>* pVecUnit = nullptr;
	size_t size = 0;

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			pVecUnit = &(*pCurGrid)[(leftTop.x + x) + (leftTop.y + y) * mGridWidth];
			size = pVecUnit->size();
			for (size_t i = 0; i < size; ++i)
			{
				if ((*pVecUnit)[i]->IsCollided(rc))
				{
					int dist = GetDistance(pos, (*pVecUnit)[i]->GetUnit()->GetPosition());
					if (dist < minDist)
					{
						minDist = dist;
						pMinUnit = (*pVecUnit)[i]->GetUnit();
					}
				}
			}
		}
	}

	if (pMinUnit)
	{
		(*pUnitID) = pMinUnit->GetUnitID();
		return true;
	}
	else
	{
		(*pUnitID) = UNIT_ID_NONE;
		return false;
	}
}

int PhysicsManager::GetDistance(const POINT& pos1, const POINT& pos2)
{
	int width = abs(pos1.x - pos2.x);
	int height = abs(pos1.y - pos2.y);

	int maxDist = max(width, height);
	int minDist = min(width, height);

	return (maxDist - minDist) + (int)((Fix)minDist * (Fix)1.4f);
}

int PhysicsManager::GetDistance(const Vector2& pos1, const Vector2& pos2)
{
	return GetDistance(POINT{ pos1.x, pos1.y }, POINT{ pos2.x, pos2.y });
}

int PhysicsManager::GetDistance(Unit* pUnit, const UnitID& targetUnitID)
{
	POINT targetPos;
	if (UNIT->GetUnitPosition(targetUnitID, targetPos))
	{
		return GetDistance(POINT{ pUnit->GetPosition().x, pUnit->GetPosition().y }, targetPos);
	}
	return -1;
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
