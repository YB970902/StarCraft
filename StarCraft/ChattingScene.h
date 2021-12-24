#pragma once
#include "Scene.h"
#include "Protocol.h"

class TextGizmo;
class ChattingScene : public Scene
{
	TextGizmo* mpText = nullptr;
	TextGizmo* mpNotice = nullptr;
	bool mbIsGetID = false;

	wchar_t mName[MAX_NAME_LEN] = { '\0', };
	int mNameIndex = 0;
	bool mbIsChoicedName = false;

	bool mbIsPrintNotice = false;
	float mElapsedNoticeTime = 0.0f;
	float mMaxNoticeTime = 1.0f;
public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update() override;

	virtual void ReceiveMessage(Message* pMsg) override;
};

