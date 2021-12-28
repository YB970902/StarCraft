#pragma once

class LobbyScene : public IScene
{
private:
	struct RoomData
	{
		RoomData() = default;
		RoomData(MsgRoomInfo* pInfo)
			: RoomName{ pInfo->Title }, CurCount{ pInfo->CurCount }, MaxCount{ pInfo->MaxCount }, RoomID{ pInfo->RoomID } { }

		room_id RoomID = DEFAULT_ROOM_ID;
		std::wstring RoomName = TEXT("");
		int CurCount = 0;
		int MaxCount = 0;
	};

	struct ChatData
	{
		ChatData(std::wstring name, std::wstring content) : Name{ name }, Content{ content } {}
		std::wstring Name;
		std::wstring Content;
	};

	wchar_t mChat[MAX_TEXT_LEN] = { '\0', };
	int mChatIndex = 0;

	std::list<ChatData> mListChatContent;
	std::vector<RoomData> mVecRoom;
	std::vector<RoomData> mVecRoomReceiving;
	int mReceivingCount = 0;

	bool mbIsWaitingJoinRoom = false;

	const wchar_t* mpNoticeText = TEXT("");
	fix mElapsedNoticeTime = 0;
	fix mMaxNoticeTime = 1.0f;
	bool mbIsShowNotice = false;

public:
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

	virtual void Notice(Message* pMsg) override;
private:
	void AddChat(std::wstring name, std::wstring chat);
	void AddRoomInfo(MsgRoomInfo* pMsg);
};