#include "stdafx.h"
#include "Room.h"
#include "Server.h"
#include "Session.h"

Room::Room(Server* pServer, int maxCount)
	:mpServer{ pServer }, mMaxPeopleCount{ maxCount }
{
}

Room::~Room()
{
}

void Room::ProcessMessage(user_id userID, char* pData)
{
	Message* pHeader = (Message*)pData;
	switch (pHeader->Tag)
	{
	case eMessageTag::RoomText:
	{
		MsgRoomText* pPacket = (MsgRoomText*)pData;
		for (int i = 0; i < mVecSession.size(); ++i)
		{
			if (mVecSession[i]->GetUserID() == userID)
			{
				continue;
			}

			mVecSession[i]->PostSend(false, pPacket->Size, pData);
		}
	}
	break;
	}
}

bool Room::UserJoin(Session* pSession)
{
	if (mVecSession.size() >= mMaxPeopleCount)
	{
		MsgRoomJoinFail msg;
		pSession->PostSend(false, msg.Size, (char*)&msg);
		return false;
	}

	mVecSession.push_back(pSession);
	MsgRoomJoin msg;
	sprintf_s(msg.Name, MAX_NAME_LEN - 1, "%s", pSession->GetName());

	for (int i = 0; i < mVecSession.size(); ++i)
	{
		if (mVecSession[i] == pSession) { continue; }
		mVecSession[i]->PostSend(false, msg.Size, (char*)&msg);
	}

	return true;
}

void Room::UserExit(Session* pSession)
{

}
