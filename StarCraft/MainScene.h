#pragma once
#include "Scene.h"

class UIGameObject;
class MainScene : public Scene
{
private:
	UIGameObject* mpBackground = nullptr;

	UIGameObject* mpBtnSingle = nullptr;
	UIGameObject* mpBtnOnSingle = nullptr;

	UIGameObject* mpBtnMulti = nullptr;
	UIGameObject* mpBtnOnMulti = nullptr;
	UIGameObject* mpMultiTail = nullptr;

	UIGameObject* mpBtnEditor = nullptr;
	UIGameObject* mpBtnOnEditor = nullptr;

	UIGameObject* mpBtnExit = nullptr;
	UIGameObject* mpBtnOnExit = nullptr;
public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update() override;
};

