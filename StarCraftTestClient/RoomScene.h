#pragma once

class RoomScene : public IScene
{
private:
	wchar_t mChat[MAX_TEXT_LEN] = { '\0', };
	int mChatIndex = 0;
	struct ChatData
	{
		ChatData(std::wstring name, std::wstring content) : Name{ name }, Content{ content } {}
		std::wstring Name;
		std::wstring Content;
	};
	std::list<ChatData> mListChatContent;

	bool mbIsExitRoom = false;

public:
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Notice(Message* pMsg) override;
private:
	void AddChat(std::wstring name, std::wstring chat);
};