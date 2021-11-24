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
	char mTitle[MAX_NAME_LEN] = { 0, };

	int mMaxPeopleCount = 0;
public:
	Room(Server* pServer, room_id roomID, const char* pTitle, int maxCount);
	~Room();

	void UserJoin(Session* pSession);
	void UserExit(Session* pSession);

	void ProcessMessage(user_id userID, char* pData);

	inline room_id GetRoomID() { return mRoomID; }
	inline const char* GetTitle() { return mTitle; }
	inline void SetTitle(const char* title) { sprintf_s(mTitle, MAX_NAME_LEN - 1, "%s", title); }

	inline int GetMaxPeopleCount() { return mMaxPeopleCount; }
	inline int GetCurPeopleCount() { return mVecSession.size(); }

	inline bool IsFull() { return (GetCurPeopleCount() >= GetMaxPeopleCount()); }
};