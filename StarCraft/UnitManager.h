#pragma once
#include "Singleton.h"

#define UNIT UnitManager::GetInstance()

static const int MAX_UNIT_COUNT = 200;

class Scene;
class Unit;
class IObserver;
class UnitManager : public Singleton<UnitManager>
{
	Scene* mpScene = nullptr;
	unordered_map<UnitID, Unit*> mMapUnit;

	queue<int> mQueRedTeamIDAllocater;
	queue<int> mQueBlueTeamIDAllocater;

	vector<Unit*> mVecRemovedUnit;
public:
	void Init(Scene* pScene);
	void Release();
	void Update();

	void CommandCreateUnit(eTeamTag teamTag, eUnitTag unitTag, int posX, int posY);
	void CommandAttackUnit(UnitID ID, UnitID targetID);
	void CommandAttackGround(UnitID ID, const POINT& pos);
	void CommandMoveUnit(UnitID ID, const POINT& pos);
	void CommandStopUnit(UnitID ID);

	void ExecuteCreateUnit(eTeamTag teamTag, eUnitTag unitTag, int posX, int posY);
	void ExecuteAttackUnit(UnitID ID, UnitID targetID);
	void ExecuteAttackGround(UnitID ID, int posX, int posY);
	void ExecuteMoveUnit(UnitID ID, int posX, int posY);
	void ExecuteStopUnit(UnitID ID);

	void SetSelectUnit(UnitID ID, bool set);

	bool GetUnitPosition(UnitID ID, POINT& position);
	eUnitTag GetUnitTag(UnitID ID);

	bool SetTargetID(UnitID ID, UnitID targetID);
	IObserver* GetObserver(UnitID ID);
	bool BeginChase(UnitID ID, UnitID TargetID);
	bool EndChase(UnitID ID, UnitID TargetID);
	bool Attack(UnitID ID, UnitID TargetID);

	void Dead(UnitID ID);
};