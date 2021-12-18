#pragma once
#include "Singleton.h"

#define PHYSICS PhysicsManager::GetInstance()

class Unit;
class ColliderComponent;
class PhysicsManager : public Singleton<PhysicsManager>
{
private:
	const int GRID_SIZE = 100;
	vector<vector<ColliderComponent*>> mVecRedTeamObjectGrid;
	vector<vector<ColliderComponent*>> mVecBlueTeamObjectGrid;

	int mGridWidth = 0;
	int mGridHeight = 0;
public:
	void Init();
	void Release();

	void InitLayerSize(int width, int height);

	void AddCollider(ColliderComponent* pCollider, const Vector2& pos, const Vector2& size);
	void RemoveCollider(ColliderComponent* pCollider, const Vector2& pos, const Vector2& size);

	void ProcessObjectMove(ColliderComponent* pCollider, const Vector2& prevPos, const Vector2& size);

	bool GetUnit(eTeamTag teamTag, const POINT& pos, Unit** ppUnit);
private:
	void AddColliderByIndex(ColliderComponent* pCollider, const POINT& leftTop, const POINT& rightBottom);
	void RemoveColliderByIndex(ColliderComponent* pCollider, const POINT& leftTop, const POINT& rightBottom);
};

