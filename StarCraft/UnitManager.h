#pragma once
#include "Singleton.h"

#define UNIT UnitManager::GetInstance()

class Scene;
class Unit;
class UnitManager : public Singleton<UnitManager>
{
	Scene* mpScene = nullptr;
	unordered_map<UnitID, Unit*> mMapUnit;
public:
	void Init(Scene* pScene);
	void Release();
	void Update();

	void CreateUnit(eTeamTag teamTag, eUnitTag unitTag, Fix posX, Fix posY, UnitID ID);
	void RemoveUnit(UnitID ID);

	void AttackUnit(UnitID ID, UnitID targetID);
	void AttackGround(UnitID ID, const POINT& pos);
	void MoveUnit(UnitID ID, const POINT& pos);
	void StopUnit(UnitID ID);

	void SetSelectUnit(UnitID ID, bool set);
	void SetTargetID(UnitID ID, UnitID targetID);

	bool GetUnitPosition(UnitID ID, POINT& position);
};