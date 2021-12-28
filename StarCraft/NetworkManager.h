#pragma once
#include "Singleton.h"
#include "Protocol.h"

enum class eCommandTag
{
	None,
	Create,
	AttackUnit,
	AttackGround,
	Move,
	Stop,
};

struct Command
{
	eCommandTag Tag = eCommandTag::None;
	UnitID ID = UNIT_ID_NONE;
	int Turn = -2;
};

struct CommandInfo
{
	int Count = 0;
	vector<Command*> Commands;
};

struct Message;
class Client;
class NetworkManager : public Singleton<NetworkManager>
{
	Client* mpClient = nullptr;

	shared_ptr<boost::asio::io_service> mIOService = nullptr;
	shared_ptr<boost::thread> mThread = nullptr;

	bool mbIsReceiving = false;
	bool mbIsSending = false;
	queue<Message*> mQueMessage;

	wchar_t mPlayerName[MAX_NAME_LEN] = { '\0' };
	user_id mPlayerID = DEFAULT_USER_ID;

	wchar_t mRoomName[MAX_NAME_LEN] = { '\0' };

	int mRoomCurCount = 0;
	int mRoomMaxCount = 0;

	bool mbIsInit = false;

	bool mbIsGameStart = false;

	const int MAX_SUB_TURN = 3;
	int mCurSubTurn = 0;
	int mCurTurn = -2;
	unordered_map<int, CommandInfo> mMapTurn;
	unordered_map<int, CommandInfo> mMapOtherTurn;
	vector<Command*> mVecCommand;
	vector<Command*> mVecTempCommand;

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

	// For LockStep

	void AddCommand(Command* pCmd);

	void InitGame();
	void ReleaseGame();
	bool UpdateGame();
private:
	void ExecuteCommand(Command* pCmd);

	void SendCommand();
	void EraseAllCommand();

	void FindTempData(int turn, CommandInfo& info);
	Command* MessageToCommand(Message* pMsg);
	void ProcessUnitMessage(Message* pMsg);
};

struct CreateCommand : public Command
{
	CreateCommand(eTeamTag teamTag, eUnitTag unitTag, int posX, int posY)
		:TeamTag{ teamTag }, UnitTag{ unitTag }, PosX{ posX }, PosY{ posY }
	{
		Tag = eCommandTag::Create;
	}
	eTeamTag TeamTag = eTeamTag::RED_TEAM;
	eUnitTag UnitTag = eUnitTag::Marine;
	int PosX = 0;
	int PosY = 0;
};

struct AttackUnitCommand : public Command
{
	AttackUnitCommand(UnitID id, UnitID targetID)
		: TargetID{ targetID }
	{
		ID = id;
		Tag = eCommandTag::AttackUnit;
	}
	UnitID TargetID = UNIT_ID_NONE;
};

struct AttackGroundCommand : public Command
{
	AttackGroundCommand(UnitID id, const POINT& pos)
		: PosX{ pos.x }, PosY{ pos.y }
	{
		ID = id;
		Tag = eCommandTag::AttackGround;
	}
	int PosX = 0;
	int PosY = 0;
};

struct MoveCommand : public Command
{
	MoveCommand(UnitID id, const POINT& pos)
		:PosX{ pos.x }, PosY{ pos.y }
	{
		ID = id;
		Tag = eCommandTag::Move;
	}
	int PosX = 0;
	int PosY = 0;
};

struct StopCommand : public Command
{
	StopCommand(UnitID id)
	{
		ID = id;
		Tag = eCommandTag::Stop;
	}
};