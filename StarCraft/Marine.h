#pragma once
#include "Unit.h"

class MarineModel;
class Marine : public Unit
{
private:
	MarineModel* mpModel = nullptr;

public:
	Marine(eTeamTag teamTag, UnitID ID, const float* arrColor);
	virtual ~Marine();
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;

	virtual void ChangeAnimation(eAnimationTag animTag);
};

