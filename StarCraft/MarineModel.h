#pragma once
#include "UnitModel.h"

class MarineModel : public UnitModel
{
public:
	MarineModel();
	virtual ~MarineModel();

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
};

