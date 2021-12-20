#pragma once
#include "Singleton.h"

class Unit;
class LineGizmo;
class UserManager : public Singleton<UserManager>
{
private:
	static const int MAX_SELECT_UNIT = 12;
	static const int LINE_SIZE = 4;

	eTeamTag mTeamTag;
	eTeamTag mEnemyTeamTag;

	vector<UnitID> mVecSelectedUnit;

	bool mbIsDrag = false;
	POINT mDragStartPos = { 0, 0 };
	LineGizmo* mpArrLineGizmo[LINE_SIZE];

public:
	void Init(eTeamTag teamTag);
	void Release();
	void Update();

	inline eTeamTag GetTeamTag() { return mTeamTag; }

	void OnUnitRemoved(UnitID ID);

private:
	void UpdateDragLine();
	void SetIsDrag(bool set);

	void ChangeSelectUnit(UnitID ID);
	void ChangeSelectDragUnit(const vector<UnitID>& vecUnit);
};

