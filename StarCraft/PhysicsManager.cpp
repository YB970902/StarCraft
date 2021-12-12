#include "stdafx.h"
#include "PhysicsManager.h"
#include "GameObject.h"
#include "RendererComponent.h"
#include "TransformComponent.h"

void PhysicsManager::Init()
{
}

void PhysicsManager::Release()
{
}

void PhysicsManager::ProcessObjectMove(GameObject* pObject, const D2D_RECT_F& rect, POINT& leftTop, POINT& rightBottom, bool isInit)
{
	POINT tempLeftTop;
	POINT tempRightBottom;

	if (!isInit)
	{
		tempLeftTop = leftTop;
		tempRightBottom = rightBottom;
	}

	leftTop.x = rect.left / GRID_SIZE;
	leftTop.y = rect.top / GRID_SIZE;

	rightBottom.x = rect.right / GRID_SIZE;
	rightBottom.y = rect.bottom / GRID_SIZE;

	if (!isInit)
	{
		if (tempLeftTop.x == leftTop.x && tempLeftTop.y == leftTop.y &&
			tempRightBottom.x == rightBottom.x && tempRightBottom.y == rightBottom.y)
		{
			return;
		}
		else
		{
			int width = tempRightBottom.x - tempLeftTop.x + 1;
			int height = tempRightBottom.y - tempLeftTop.y + 1;

			for (int x = 0; x < width; ++x)
			{
				for (int y = 0; y < height; ++y)
				{
					mMapObjectGrid[tempLeftTop.x + x][tempLeftTop.y + y].erase(pObject);
				}
			}
		}
	}

	int width = rightBottom.x - leftTop.x + 1;
	int height = rightBottom.y - leftTop.y + 1;

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			mMapObjectGrid[leftTop.x + x][leftTop.y + y].insert(pObject);
		}
	}
}

void PhysicsManager::ProcessCameraMove(const RECT& rect, POINT& leftTop, POINT& rightBottom, bool isInit)
{
	POINT tempLeftTop;
	POINT tempRightBottom;

	if (!isInit)
	{
		tempLeftTop = leftTop;
		tempRightBottom = rightBottom;
	}

	leftTop.x = rect.left / GRID_SIZE;
	leftTop.y = rect.top / GRID_SIZE;

	rightBottom.x = rect.right / GRID_SIZE;
	rightBottom.y = rect.bottom / GRID_SIZE;

	if (!isInit)
	{
		if (tempLeftTop.x == leftTop.x && tempLeftTop.y == leftTop.y &&
			tempRightBottom.x == rightBottom.x && tempRightBottom.y == rightBottom.y)
		{
			return;
		}
		else
		{
			int width = tempRightBottom.x - tempLeftTop.x + 1;
			int height = tempRightBottom.y - tempLeftTop.y + 1;

			for (int x = 0; x < width; ++x)
			{
				for (int y = 0; y < height; ++y)
				{
					set<GameObject*>& grid = mMapObjectGrid[leftTop.x + x][leftTop.y + y];
					for (auto it = grid.begin(); it != grid.end(); ++it)
					{
						(*it)->GetRenderer()->SetIsRender(false);
					}
				}
			}
		}
	}

	int width = rightBottom.x - leftTop.x + 1;
	int height = rightBottom.y - leftTop.y + 1;

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			set<GameObject*>& grid = mMapObjectGrid[leftTop.x + x][leftTop.y + y];
			for (auto it = grid.begin(); it != grid.end(); ++it)
			{
				(*it)->GetRenderer()->SetIsRender(true);
			}
		}
	}
}