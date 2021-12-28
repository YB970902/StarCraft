#pragma once
#include "Scene.h"
#include "Protocol.h"

enum class eChattingState
{
	Login,
	Lobby,
	CreateRoom,
	Room,
};

class ChattingState;
class TextGizmo;
class ChattingScene : public Scene
{
	unordered_map<eChattingState, ChattingState*> mState;
	ChattingState* mpCurState = nullptr;
public:
	ChattingScene() = default;
	virtual ~ChattingScene() = default;

	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update() override;

	void ChangeState(eChattingState stateTag);

	virtual void ReceiveMessage(Message* pMsg) override;
};

class ChattingState
{
protected:
	ChattingScene* mpScene = nullptr;
public:
	ChattingState() = default;
	virtual ~ChattingState() = default;

	void Init(ChattingScene* pScene) { mpScene = pScene; }

	virtual void Enter() abstract;
	virtual void Exit() abstract;
	virtual void Update() abstract;
	virtual void ReceiveMessage(Message* pMsg) abstract;
};

class LobbyState : public ChattingState
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

	TextGizmo* mpUI = nullptr;
	TextGizmo* mpText = nullptr;

	wchar_t mChat[MAX_TEXT_LEN] = { '\0', };
	int mChatIndex = 0;

	std::list<ChatData> mListChatContent;
	std::vector<RoomData> mVecRoom;
	std::vector<RoomData> mVecRoomReceiving;
	int mReceivingCount = 0;

	bool mbIsWaitingJoinRoom = false;

	const wchar_t* mpNoticeText = TEXT("");
	Fix mElapsedNoticeTime = 0;
	Fix mMaxNoticeTime = 1.0f;
	bool mbIsShowNotice = false;

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update() override;
	virtual void ReceiveMessage(Message* pMsg) override;

	void AddChat(std::wstring name, std::wstring chat);
	void AddRoomInfo(MsgRoomInfo* pMsg);

	void SetUIText();
	void SetText();
};

class LoginState : public ChattingState
{
private:
	TextGizmo* mpText = nullptr;
	TextGizmo* mpNotice = nullptr;

	wchar_t mName[MAX_NAME_LEN] = { '\0', };
	int mNameIndex = 0;
	bool mbIsGetID = false;
	bool mbIsChoicedName = false;

	bool mbIsPrintNotice = false;
	float mElapsedNoticeTime = 0.0f;
	float mMaxNoticeTime = 1.0f;
public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update() override;
	virtual void ReceiveMessage(Message* pMsg) override;

	void SetName();
};

class CreateRoomState : public ChattingState
{
public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update() override;
	virtual void ReceiveMessage(Message* pMsg) override;
};

class RoomState : public ChattingState
{
public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update() override;
	virtual void ReceiveMessage(Message* pMsg) override;
};