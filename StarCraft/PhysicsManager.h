#pragma once
#include "Singleton.h"

#define PHYSICS PhysicsManager::GetInstance()

class PhysicsManager : public Singleton<PhysicsManager>
{
private:
	const int GRID_SIZE = 100;
	unordered_map<int, unordered_map<int, set<GameObject*>>> mMapObjectGrid;
public:
	void Init();
	void Release();

	void ProcessObjectMove(GameObject* pObject, const D2D_RECT_F& rect, POINT& leftTop, POINT& rightBottom, bool isInit = false);
	void ProcessCameraMove(const RECT& rect, POINT& leftTop, POINT& rightBottom, bool isInit = false);
};

