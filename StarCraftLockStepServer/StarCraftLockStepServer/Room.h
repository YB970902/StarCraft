#pragma once

class Session;
class Server;
class Room
{
protected:
	Server* mpServer = nullptr;

	std::vector<Session*> mVecSession;
	bool mbIsGameStarted = false;

	room_id mRoomID = DEFAULT_ROOM_ID;
	wchar_t mTitle[MAX_NAME_LEN] = { 0, };

	int mMaxPeopleCount = 0;
public:
	Room(Server* pServer, room_id roomID, const wchar_t* pTitle, int maxCount);
	~Room();

	void UserJoin(Session* pSession);
	void UserExit(Session* pSession);

	void GameStart();

	void ProcessMessage(user_id userID, char* pData);

	void SendToOther(Session* pSession, size_t size, char* pData);

	inline room_id GetRoomID() { return mRoomID; }
	inline const wchar_t* GetTitle() { return mTitle; }
	inline void SetTitle(const wchar_t* title) { swprintf_s(mTitle, MAX_NAME_LEN - 1, TEXT("%s"), title); }

	inline int GetMaxPeopleCount() { return mMaxPeopleCount; }
	inline int GetCurPeopleCount() { return mVecSession.size(); }

	inline bool IsFull() { return (GetCurPeopleCount() >= GetMaxPeopleCount()); }

	inline bool IsGameStarted() { return mbIsGameStarted; }
};