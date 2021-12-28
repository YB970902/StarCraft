#pragma once
#include "UnitModel.h"

class GoliathHeadModel : public UnitModel
{
public:
	GoliathHeadModel();
	virtual ~GoliathHeadModel();

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
};