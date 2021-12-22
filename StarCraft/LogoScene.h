#pragma once
#include "Scene.h"

class UIGameObject;
class LogoScene : public Scene
{
private:
	Fix mElapsedTime = 0;
	Fix mDurationSceneChangeTime = 2.0f;

	UIGameObject* mpBackground = nullptr;
public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update() override;
};