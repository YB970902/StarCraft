#pragma once
#include "Scene.h"

class GameObject;
class MainScene : public Scene
{
private:
	GameObject* mpBackground = nullptr;

	GameObject* mpBtnSingle = nullptr;
	GameObject* mpBtnOnSingle = nullptr;

	GameObject* mpBtnMulti = nullptr;
	GameObject* mpBtnOnMulti = nullptr;
	GameObject* mpMultiTail = nullptr;

	GameObject* mpBtnEditor = nullptr;
	GameObject* mpBtnOnEditor = nullptr;

	GameObject* mpBtnExit = nullptr;
	GameObject* mpBtnOnExit = nullptr;
public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update() override;
};

