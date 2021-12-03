#pragma once
#include "Scene.h"

class Unit;
class TestScene : public Scene
{
protected:
	Unit* pUnit = nullptr;
public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update() override;
};