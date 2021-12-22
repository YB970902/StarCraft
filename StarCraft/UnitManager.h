#pragma once
#include "Singleton.h"

#define UNIT UnitManager::GetInstance()

class Scene;
class Unit;
class IObserver;
class UnitManager : public Singleton<UnitManager>
{
	Scene* mpScene = nullptr;
	unordered_map<UnitID, Unit*> mMapUnit;

	vector<Unit*> mVecRemovedUnit;
public:
	void Init(Scene* pScene);
	void Release();
	void Update();

	void CreateUnit(eTeamTag teamTag, eUnitTag unitTag, Fix posX, Fix posY, UnitID ID);
	void RemoveUnit(UnitID ID);

	void CommandAttackUnit(UnitID ID, UnitID targetID);
	void CommandAttackGround(UnitID ID, const POINT& pos);
	void CommandMoveUnit(UnitID ID, const POINT& pos);
	void CommandStopUnit(UnitID ID);

	void SetSelectUnit(UnitID ID, bool set);

	bool GetUnitPosition(UnitID ID, POINT& position);

	bool SetTargetID(UnitID ID, UnitID targetID);
	IObserver* GetObserver(UnitID ID);
	bool BeginChase(UnitID ID, UnitID TargetID);
	bool EndChase(UnitID ID, UnitID TargetID);
	bool Attack(UnitID ID, UnitID TargetID);

	void Dead(UnitID ID);
};