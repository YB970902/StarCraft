#pragma once
#include "Singleton.h"

class UserManager : public Singleton<UserManager>
{
private:
	wchar_t mPlayerName[MAX_NAME_LEN] = { '\0' };
	user_id mPlayerID = DEFAULT_USER_ID;

	wchar_t mRoomName[MAX_NAME_LEN] = { '\0' };

	int mRoomCurCount = 0;
	int mRoomMaxCount = 0;

public:
	const wchar_t* GetPlayerName() { return mPlayerName; }
	void SetPlayerName(const wchar_t* pUserName) { swprintf_s(mPlayerName, MAX_NAME_LEN - 1, TEXT("%s"), pUserName); }
	int GetPlayerID() { return mPlayerID; }
	void SetPlayerID(int playerID) { mPlayerID = playerID; }

	const wchar_t* GetRoomName() { return mRoomName; }
	void SetRoomName(const wchar_t* pRoomName) { swprintf_s(mRoomName, MAX_NAME_LEN - 1, TEXT("%s"), pRoomName); }

	int GetCurrentRoomCount() { return mRoomCurCount; }
	void SetCurrentRoomCount(int count) { mRoomCurCount = count; }

	int GetMaxRoomCount() { return mRoomMaxCount; }
	void SetMaxRoomCount(int count) { mRoomMaxCount = count; }
};

