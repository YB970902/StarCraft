#pragma once

class CreateRoomScene : public IScene
{
	wchar_t mTitle[MAX_NAME_LEN] = { '\0' };
	int mTitleIndex = 0;
	int mMaxCount = 0;
	bool mbIsCreateRoom = false;
public:
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

	virtual void Notice(Message* pMsg) override;
};

