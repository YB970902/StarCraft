#pragma once
#include "Unit.h"

class UnitModel;
class Goliath : public Unit
{
private:
	UnitModel* mpHeadModel = nullptr;
	UnitModel* mpLegModel = nullptr;

public:
	Goliath(eTeamTag teamTag, UnitID ID, const float* arrColor);
	virtual ~Goliath();
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;

	virtual void ChangeAnimation(eAnimationTag animTag);
};