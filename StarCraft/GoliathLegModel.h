#pragma once
#include "UnitModel.h"

class GoliathLegModel : public UnitModel
{
public:
	GoliathLegModel();
	virtual ~GoliathLegModel();

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
};

