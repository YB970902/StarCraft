#pragma once
#include "Singleton.h"
#include "Protocol.h"

struct Message;
class Client;
class NetworkManager : public Singleton<NetworkManager>
{
	Client* mpClient = nullptr;

	boost::asio::io_service mIOService;
	shared_ptr<boost::thread> mThread = nullptr;

	bool mbIsReceiving = false;
	bool mbIsSending = false;
	queue<Message*> mQueMessage;

	wchar_t mPlayerName[MAX_NAME_LEN] = { '\0' };
	user_id mPlayerID = DEFAULT_USER_ID;

	wchar_t mRoomName[MAX_NAME_LEN] = { '\0' };

	int mRoomCurCount = 0;
	int mRoomMaxCount = 0;

public:
	void Init();
	void Release();
	void Update();

	bool IsSending() { return mbIsSending; }

	void SetPlayerName(const wchar_t* pPlayerName);

	void SendChat(const wchar_t* pChat);
	void CreateRoom(const wchar_t* pTitle, int maxCount);
	void RefreshRoomInfo();
	void RequestJoinRoom(room_id roomID);

	void ReceiveMessage(Message* pMsg);
	void ReceiveEnd();

	void ProcessMessage(Message* pMsg);

	const wchar_t* GetPlayerName() { return mPlayerName; }
	int GetPlayerID() { return mPlayerID; }
	void SetPlayerID(int playerID) { mPlayerID = playerID; }

	const wchar_t* GetRoomName() { return mRoomName; }
	void SetRoomName(const wchar_t* pRoomName) { swprintf_s(mRoomName, MAX_NAME_LEN - 1, TEXT("%s"), pRoomName); }

	int GetCurrentRoomCount() { return mRoomCurCount; }
	void SetCurrentRoomCount(int count) { mRoomCurCount = count; }

	int GetMaxRoomCount() { return mRoomMaxCount; }
	void SetMaxRoomCount(int count) { mRoomMaxCount = count; }
};