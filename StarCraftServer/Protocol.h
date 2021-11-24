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
	UnitMove,
	UnitChase,
	UnitMovingAttack,
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
	char Name[MAX_NAME_LEN] = { 0, };
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
	char Title[MAX_NAME_LEN] = { 0, };
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
	char Name[MAX_NAME_LEN] = { 0, };
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
	char Name[MAX_NAME_LEN] = { 0, };
};

struct MsgRoomCreate : public Message
{
	MsgRoomCreate()
	{
		Tag = eMessageTag::RoomCreate;
		Size = sizeof(MsgRoomCreate);
	}
	char Title[MAX_NAME_LEN] = { 0, };
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
	char Title[MAX_NAME_LEN] = { 0, };
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
	char Name[MAX_NAME_LEN] = { 0, };
	char Text[MAX_TEXT_LEN] = { 0, };
};

struct MsgUnitMove : public Message
{
	MsgUnitMove()
	{
		Tag = eMessageTag::UnitMove;
		Size = sizeof(MsgUnitMove);
	}
	user_id UserID = 0;
	unit_id UnitID = 0;
};

struct MsgUnitChase : public Message
{
	MsgUnitChase()
	{
		Tag = eMessageTag::UnitChase;
		Size = sizeof(MsgUnitChase);
	}
	user_id UserID = 0;
	unit_id UnitID = 0;
	unit_id TargetUnitID = 0;
};