#pragma once
#include "Scene.h"

class TextGizmo;
class MatchingScene : public Scene
{
	TextGizmo* mpText = nullptr;
public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update() override;
	virtual void ReceiveMessage(Message* pMsg) override;
};