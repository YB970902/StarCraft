#pragma once
#include "Singleton.h"

class UserManager : public Singleton<UserManager>
{
private:
	char mPlayerName[MAX_NAME_LEN] = { '\0' };
	user_id mPlayerID = DEFAULT_USER_ID;

	char mRoomName[MAX_NAME_LEN] = { '\0' };

	int mRoomCurCount = 0;
	int mRoomMaxCount = 0;

public:
	const char* GetPlayerName() { return mPlayerName; }
	void SetPlayerName(const char* pUserName) { sprintf_s(mPlayerName, MAX_NAME_LEN - 1, "%s", pUserName); }
	int GetPlayerID() { return mPlayerID; }
	void SetPlayerID(int playerID) { mPlayerID = playerID; }

	const char* GetRoomName() { return mRoomName; }
	void SetRoomName(const char* pRoomName) { sprintf_s(mRoomName, MAX_NAME_LEN - 1, "%s", pRoomName); }

	int GetCurrentRoomCount() { return mRoomCurCount; }
	void SetCurrentRoomCount(int count) { mRoomCurCount = count; }

	int GetMaxRoomCount() { return mRoomMaxCount; }
	void SetMaxRoomCount(int count) { mRoomMaxCount = count; }
};

