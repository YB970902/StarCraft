#pragma once
#include "Scene.h"

class MultiGameScene : public Scene
{
public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update() override;
	virtual void ReceiveMessage(Message* pMsg) override;
};
