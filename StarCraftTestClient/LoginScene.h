#pragma once

class LoginScene : public IScene
{
private:
	wchar_t mName[MAX_NAME_LEN] = { '\0', };
	int mNameIndex = 0;
	bool mbIsChoicedName = false;
	bool mbIsGetID = false;

	bool mbIsPrintNotice = false;
	fix mElapsedNoticeTime = 0.0f;
	fix mMaxNoticeTime = 1.0f;

	const wchar_t* mNoticeText = TEXT("");
public:
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

	virtual void Notice(Message* pMsg) override;
};
