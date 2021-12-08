#pragma once
#include "Scene.h"

const Fix CAMERA_MOVING_SPEED = 500;
const Fix CAMERA_FAST_MOVING_SPEED = 1000;

class TextGizmo;
class DetailMap;
class PathFinder;
class PathFindingScene : public Scene
{
private:
	DetailMap* mpDetailMap = nullptr;
	PathFinder* mpPathFinder = nullptr;
	int mMapWidth = 1000;
	int mMapHeight = 1000;
public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update() override;
};
