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

void PhysicsManager::InitLayerSize(int width, int height)
{
}

void PhysicsManager::ProcessObjectMove(GameObject* pObject, const D2D_RECT_F& rect, POINT& leftTop, POINT& rightBottom, bool isInit)
{
	//if (isInit)
	//{
	//	leftTop.x = rect.left / GRID_SIZE;
	//	leftTop.y = rect.top / GRID_SIZE;
	//
	//	rightBottom.x = rect.right / GRID_SIZE;
	//	rightBottom.y = rect.bottom / GRID_SIZE;
	//
	//	AddObject(pObject, leftTop, rightBottom);
	//
	//	return;
	//}
	//
	//POINT prevLeftTop = leftTop;
	//POINT prevRightBottom = rightBottom;
	//
	//leftTop.x = rect.left / GRID_SIZE;
	//leftTop.y = rect.top / GRID_SIZE;
	//
	//rightBottom.x = rect.right / GRID_SIZE;
	//rightBottom.y = rect.bottom / GRID_SIZE;
	//
	//// 너무 많이 이동한 경우
	//if (prevLeftTop.x > rightBottom.x || prevRightBottom.x < leftTop.x ||
	//	prevLeftTop.y > rightBottom.y || prevRightBottom.y < leftTop.y)
	//{
	//	EraseObject(pObject, prevLeftTop, prevRightBottom);
	//	AddObject(pObject, leftTop, rightBottom);
	//
	//	return;
	//}
	//
	//// X축 이동 처리
	//
	//if (leftTop.x > prevLeftTop.x)
	//{
	//	EraseObject(pObject, POINT{ prevLeftTop.x, prevLeftTop.y }, POINT{ leftTop.x - 1, prevRightBottom.y });
	//}
	//else if (leftTop.x < prevLeftTop.x)
	//{
	//	AddObject(pObject, POINT{ leftTop.x, leftTop.y }, POINT{ prevLeftTop.x - 1, rightBottom.y });
	//}
	//
	//if (rightBottom.x > prevRightBottom.x)
	//{
	//	AddObject(pObject, POINT{ prevRightBottom.x + 1, leftTop.y }, POINT{ rightBottom.x, rightBottom.y });
	//}
	//else if (rightBottom.x < prevRightBottom.x)
	//{
	//	EraseObject(pObject, POINT{ rightBottom.x + 1, prevLeftTop.x }, POINT{ prevRightBottom.x, prevRightBottom.y });
	//}
	//
	//// Y축 이동 처리
	//
	//if (leftTop.y > prevLeftTop.y)
	//{
	//	EraseObject(pObject, POINT{ prevLeftTop.x, prevLeftTop.y }, POINT{ prevRightBottom.x, leftTop.y - 1 });
	//}
	//else if (leftTop.y < prevLeftTop.y)
	//{
	//	AddObject(pObject, POINT{ leftTop.x, leftTop.y }, POINT{ rightBottom.x, prevLeftTop.y - 1 });
	//}
	//
	//if (rightBottom.y > prevRightBottom.y)
	//{
	//	AddObject(pObject, POINT{ leftTop.x, prevRightBottom.y + 1 }, POINT{ rightBottom.x, rightBottom.y });
	//}
	//else if (rightBottom.y < prevRightBottom.y)
	//{
	//	EraseObject(pObject, POINT{ prevLeftTop.x, rightBottom.y + 1 }, POINT{ prevRightBottom.x, prevRightBottom.y });
	//}
}

void PhysicsManager::ProcessCameraMove(const RECT& rect, POINT& leftTop, POINT& rightBottom, bool isInit)
{
	//if (isInit)
	//{
	//	leftTop.x = rect.left / GRID_SIZE;
	//	leftTop.y = rect.top / GRID_SIZE;
	//
	//	rightBottom.x = rect.right / GRID_SIZE;
	//	rightBottom.y = rect.bottom / GRID_SIZE;
	//
	//	SetObjectRender(leftTop, rightBottom, true);
	//
	//	return;
	//}
	//
	//POINT prevLeftTop = leftTop;
	//POINT prevRightBottom = rightBottom;
	//
	//leftTop.x = rect.left / GRID_SIZE;
	//leftTop.y = rect.top / GRID_SIZE;
	//
	//rightBottom.x = rect.right / GRID_SIZE;
	//rightBottom.y = rect.bottom / GRID_SIZE;
	//
	//// 너무 많이 이동한 경우
	//if (prevLeftTop.x > rightBottom.x || prevRightBottom.x < leftTop.x ||
	//	prevLeftTop.y > rightBottom.y || prevRightBottom.y < leftTop.y)
	//{
	//	SetObjectRender(prevLeftTop, prevRightBottom, false);
	//	SetObjectRender(leftTop, rightBottom, true);
	//
	//	return;
	//}
	//
	//// X축 이동 처리
	//
	//if (leftTop.x > prevLeftTop.x)
	//{
	//	SetObjectRender(POINT{ prevLeftTop.x, prevLeftTop.y }, POINT{ leftTop.x - 1, prevRightBottom.y }, false);
	//}
	//else if (leftTop.x < prevLeftTop.x)
	//{
	//	SetObjectRender(POINT{ leftTop.x, leftTop.y }, POINT{ prevLeftTop.x - 1, rightBottom.y }, true);
	//}
	//
	//if (rightBottom.x > prevRightBottom.x)
	//{
	//	SetObjectRender(POINT{ prevRightBottom.x + 1, leftTop.y }, POINT{ rightBottom.x, rightBottom.y }, true);
	//}
	//else if (rightBottom.x < prevRightBottom.x)
	//{
	//	SetObjectRender(POINT{ rightBottom.x + 1, prevLeftTop.x }, POINT{ prevRightBottom.x, prevRightBottom.y }, false);
	//}
	//
	//// Y축 이동 처리
	//
	//if (leftTop.y > prevLeftTop.y)
	//{
	//	SetObjectRender(POINT{ prevLeftTop.x, prevLeftTop.y }, POINT{ prevRightBottom.x, leftTop.y - 1 }, false);
	//}
	//else if (leftTop.y < prevLeftTop.y)
	//{
	//	SetObjectRender(POINT{ leftTop.x, leftTop.y }, POINT{ rightBottom.x, prevLeftTop.y - 1 }, true);
	//}
	//
	//if (rightBottom.y > prevRightBottom.y)
	//{
	//	SetObjectRender(POINT{ leftTop.x, prevRightBottom.y + 1 }, POINT{ rightBottom.x, rightBottom.y }, true);
	//}
	//else if (rightBottom.y < prevRightBottom.y)
	//{
	//	SetObjectRender(POINT{ prevLeftTop.x, rightBottom.y + 1 }, POINT{ prevRightBottom.x, prevRightBottom.y }, false);
	//}
}

void PhysicsManager::EraseObject(GameObject* pObject, const POINT& leftTop, const POINT& rightBottom)
{
	int width = rightBottom.x - leftTop.x + 1;
	int height = rightBottom.y - leftTop.y + 1;

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			set<GameObject*>& grid = mMapObjectGrid[leftTop.x + x][leftTop.y + y];
			auto it = grid.find(pObject);
			if (it != grid.end()) { grid.erase(it); }
		}
	}
}

void PhysicsManager::AddObject(GameObject* pObject, const POINT& leftTop, const POINT& rightBottom)
{
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

void PhysicsManager::SetObjectRender(const POINT& leftTop, const POINT& rightBottom, bool isSet)
{
	int width = rightBottom.x - leftTop.x + 1;
	int height = rightBottom.y - leftTop.y + 1;

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			set<GameObject*>& grid = mMapObjectGrid[leftTop.x + x][leftTop.y + y];
			for (auto it : mMapObjectGrid[leftTop.x + x][leftTop.y + y])
			{
				it->GetRenderer()->SetIsRender(isSet);
			}
		}
	}
}
