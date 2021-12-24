#include "stdafx.h"
#include "Room.h"
#include "Server.h"
#include "Session.h"

Room::Room(Server* pServer, room_id roomID, const wchar_t* pTitle, int maxCount)
	: mpServer{ pServer }, mRoomID { roomID }, mMaxPeopleCount{ maxCount }
{
	swprintf_s(mTitle, MAX_NAME_LEN, TEXT("%s"), pTitle);
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
			if (mVecSession[i]->GetUserID() == userID) { continue; }
			mVecSession[i]->PostSend(false, pPacket->Size, pData);
		}
	}
	break;
	}
}

void Room::UserJoin(Session* pSession)
{
	mVecSession.push_back(pSession);
	MsgRoomJoin msg;
	swprintf_s(msg.Name, MAX_NAME_LEN, TEXT("%s"), pSession->GetName());

	for (int i = 0; i < mVecSession.size(); ++i)
	{
		if (mVecSession[i] == pSession) { continue; }
		mVecSession[i]->PostSend(false, msg.Size, (char*)&msg);
	}
}

void Room::UserExit(Session* pSession)
{
	MsgRoomExit msg;
	swprintf_s(msg.Name, MAX_NAME_LEN, TEXT("%s"), pSession->GetName());

	for (auto it = mVecSession.begin(); it != mVecSession.end();)
	{
		if ((*it) == pSession)
		{
			it = mVecSession.erase(it);
		}
		else
		{
			(*it)->PostSend(false, msg.Size, (char*)&msg);
			++it;
		}
	}
}
