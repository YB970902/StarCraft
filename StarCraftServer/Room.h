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
	Room(Server* pServer, int maxCount);
	~Room();

	bool UserJoin(Session* pSession);
	void UserExit(Session* pSession);

	void ProcessMessage(user_id userID, char* pData);

	const char* GetTitle() { return mTitle; }
	void SetTitle(const char* title) { sprintf_s(mTitle, MAX_NAME_LEN - 1, "%s", title); }

	int GetMaxPeopleCount() { return mMaxPeopleCount; }
	int GetCurPeopleCount() { return mVecSession.size(); }
};