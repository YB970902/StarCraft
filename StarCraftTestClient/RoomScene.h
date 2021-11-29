#pragma once

class RoomScene : public IScene
{
private:
	char mChat[MAX_TEXT_LEN] = { '\0', };
	int mChatIndex = 0;

	std::list<std::pair<std::string, std::string>> mListChatContent;

	bool mbIsExitRoom = false;

public:
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Notice(Message* pMsg) override;
private:
	void AddChat(std::string name, std::string chat);
};