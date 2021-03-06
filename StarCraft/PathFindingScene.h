#pragma once
#include "Scene.h"

const Fix CAMERA_MOVING_SPEED = 500;
const Fix CAMERA_FAST_MOVING_SPEED = 1000;

class Unit;
class PathFindingScene : public Scene
{
private:

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update() override;
};
