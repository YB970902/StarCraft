#pragma once

static const char* SERVER_ADDRESS = "121.132.120.90";
const unsigned int PORT_NUMBER = 31400;

const unsigned int MAX_RECEIVE_BUFFER_LEN = 512;

const unsigned int MAX_NAME_LEN = 17;
const unsigned int MAX_TEXT_LEN = 32;
const unsigned int MAX_TEXT_LOG_SIZE = 10;

using user_id = unsigned int;
using unit_id = unsigned int;
using room_id = unsigned int;

const unsigned int MAX_USER_COUNT = 10;
const unsigned int MAX_ROOM_COUNT = 10;
const user_id DEFAULT_USER_ID = 0;
const room_id DEFAULT_ROOM_ID = 0;

static void PrintErrorCode(const boost::system::error_code& error)
{
	if (error == boost::asio::error::eof)
	{
		std::cout << "클라이언트와 연결이 끊겼습니다" << std::endl;
	}
	else
	{
		std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
	}
}

enum class eMessageTag
{
	None,
	SetName,
	SetUserID,
	SetRoomID,
	RoomJoinRequest,
	RoomJoinFail,
	RoomJoinSuccess,
	RoomJoin,
	RoomExitRequest,
	RoomExit,
	RoomLeft,
	RoomCreate,
	RoomBanned,
	RoomUpdateRequest,
	RoomInfo,
	ReqRoomInfo,
	RoomText,
	GameStart,
	InputData,
	UnitSpawn,
	UnitMove,
	UnitMoveAlertly,
	UnitAttack,
	UnitStop,
};

struct Message
{
	eMessageTag Tag = eMessageTag::None;
	unsigned short Size = 0;
};

struct MsgSetName : public Message
{
	MsgSetName()
	{
		Tag = eMessageTag::SetName;
		Size = sizeof(MsgSetName);
	}
	wchar_t Name[MAX_NAME_LEN] = { 0, };
};

struct MsgSetUserID : public Message
{
	MsgSetUserID()
	{
		Tag = eMessageTag::SetUserID;
		Size = sizeof(MsgSetUserID);
	}
	user_id UserID = DEFAULT_USER_ID;
};

struct MsgRoomJoinRequest : public Message
{
	MsgRoomJoinRequest()
	{
		Tag = eMessageTag::RoomJoinRequest;
		Size = sizeof(MsgRoomJoinRequest);
	}
	room_id RoomID = DEFAULT_ROOM_ID;
};

struct MsgRoomJoinFail : public Message
{
	MsgRoomJoinFail()
	{
		Tag = eMessageTag::RoomJoinFail;
		Size = sizeof(MsgRoomJoinFail);
	}
};

struct MsgRoomJoinSuccess : public Message
{
	MsgRoomJoinSuccess()
	{
		Tag = eMessageTag::RoomJoinSuccess;
		Size = sizeof(MsgRoomJoinSuccess);
	}
	wchar_t Title[MAX_NAME_LEN] = { 0, };
	int CurCount = 0;
	int MaxCount = 0;
};

struct MsgRoomJoin : public Message
{
	MsgRoomJoin()
	{
		Tag = eMessageTag::RoomJoin;
		Size = sizeof(MsgRoomJoin);
	}
	wchar_t Name[MAX_NAME_LEN] = { 0, };
};

struct MsgRoomExitRequest : public Message
{
	MsgRoomExitRequest()
	{
		Tag = eMessageTag::RoomExitRequest;
		Size = sizeof(MsgRoomExitRequest);
	}
};

struct MsgRoomExit : public Message
{
	MsgRoomExit()
	{
		Tag = eMessageTag::RoomExit;
		Size = sizeof(MsgRoomExit);
	}
	wchar_t Name[MAX_NAME_LEN] = { 0, };
};

struct MsgRoomCreate : public Message
{
	MsgRoomCreate()
	{
		Tag = eMessageTag::RoomCreate;
		Size = sizeof(MsgRoomCreate);
	}
	wchar_t Title[MAX_NAME_LEN] = { 0, };
	int MaxCount = 0;
};

struct MsgRoomBanned : public Message
{
	MsgRoomBanned()
	{
		Tag = eMessageTag::RoomBanned;
		Size = sizeof(MsgRoomBanned);
	}
};

struct MsgRoomUpdateRequest : public Message
{
	MsgRoomUpdateRequest()
	{
		Tag = eMessageTag::RoomUpdateRequest;
		Size = sizeof(MsgRoomUpdateRequest);
	}
};

struct MsgRoomInfo : public Message
{
	MsgRoomInfo()
	{
		Tag = eMessageTag::RoomInfo;
		Size = sizeof(MsgRoomInfo);
	}
	int Length = 0;
	int Index = 0;
	room_id RoomID = DEFAULT_ROOM_ID;
	wchar_t Title[MAX_NAME_LEN] = { 0, };
	int CurCount = 0;
	int MaxCount = 0;
};

struct MsgReqRoomInfo : public Message
{
	MsgReqRoomInfo()
	{
		Tag = eMessageTag::ReqRoomInfo;
		Size = sizeof(MsgReqRoomInfo);
	}
};

struct MsgRoomText : public Message
{
	MsgRoomText()
	{
		Tag = eMessageTag::RoomText;
		Size = sizeof(MsgRoomText);
	}
	wchar_t Name[MAX_NAME_LEN] = { 0, };
	wchar_t Text[MAX_TEXT_LEN] = { 0, };
};

struct MsgGameStart : public Message
{
	MsgGameStart()
	{
		Tag = eMessageTag::GameStart;
		Size = sizeof(MsgGameStart);
	}
	int RandomSeed = 0;
	int TeamTag = 0;
};

struct MsgInputData : public Message
{
	MsgInputData()
	{
		Tag = eMessageTag::InputData;
		Size = sizeof(MsgInputData);
	}
	int Turn = 0;
	int Count = 0;
};

struct MsgUnitSpawn : public Message
{
	MsgUnitSpawn()
	{
		Tag = eMessageTag::UnitSpawn;
		Size = sizeof(MsgUnitSpawn);
	}
	int Turn = 0;
	int SpawnPosX = 0;
	int SpawnPosY = 0;
	int TeamTag = -1;
	int UnitTag = -1;
};

struct MsgUnitMove : public Message
{
	MsgUnitMove()
	{
		Tag = eMessageTag::UnitMove;
		Size = sizeof(MsgUnitMove);
	}
	int Turn = 0;
	UnitID ID = UNIT_ID_NONE;
	int DestPosX = 0;
	int DestPosY = 0;
};

struct MsgUnitMoveAlertly: public Message
{
	MsgUnitMoveAlertly()
	{
		Tag = eMessageTag::UnitMoveAlertly;
		Size = sizeof(MsgUnitMoveAlertly);
	}
	int Turn = 0;
	UnitID ID = UNIT_ID_NONE;
	int DestPosX = 0;
	int DestPosY = 0;
};

struct MsgUnitAttack : public Message
{
	MsgUnitAttack()
	{
		Tag = eMessageTag::UnitAttack;
		Size = sizeof(MsgUnitAttack);
	}
	int Turn = 0;
	UnitID ID = UNIT_ID_NONE;
	UnitID TargetID = UNIT_ID_NONE;
};

struct MsgUnitStop : public Message
{
	MsgUnitStop()
	{
		Tag = eMessageTag::UnitStop;
		Size = sizeof(MsgUnitStop);
	}
	int Turn = 0;
	UnitID ID = UNIT_ID_NONE;
};