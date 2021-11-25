#include "stdafx.h"
#include "Session.h"
#include "Server.h"
#include "Room.h"
#include "SessionState.h"

Session::Session(user_id userID, boost::asio::io_service& IOService, Server* pServer)
	: mUserID{ userID }, mSocket{ IOService }, mpServer{ pServer } { }

Session::~Session() { }

void Session::Init()
{
	mpRoom = nullptr;
	mPacketBufferMark = 0;
	mpState = new SessionState(this);
	mpState->Init();
}

void Session::Release()
{
	while (mQueSendData.empty() == false)
	{
		delete[] mQueSendData.front();
		mQueSendData.pop_front();
	}
	if (mpRoom)
	{
		ExitRoom();
	}
	mSocket.close();

	mpState->Release();
	delete mpState;
	mpState = nullptr;
}

void Session::PostSend(bool bIsImmediately, size_t size, char* pData)
{
	char* pSendData = nullptr;

	if (bIsImmediately)
	{
		pSendData = pData;
	}
	else
	{
		pSendData = new char[size];
		memcpy(pSendData, pData, size);

		mQueSendData.push_back(pSendData);
	}

	if (bIsImmediately == false && mQueSendData.size() > 1)
	{
		return;
	}

	boost::asio::async_write(mSocket, boost::asio::buffer(pSendData, size),
		boost::bind(&Session::HandleWrite, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)
	);
}

void Session::PostReceive()
{
	mSocket.async_read_some
	(
		boost::asio::buffer(mReceiveBuffer),
		boost::bind(&Session::HandleReceive, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)
	);
}

void Session::HandleWrite(const boost::system::error_code& error, size_t bytes_transferred)
{
	delete[] mQueSendData.front();
	mQueSendData.pop_front();

	if (mQueSendData.empty() == false)
	{
		char* pData = mQueSendData.front();

		Message* pHeader = (Message*)pData;

		PostSend(true, pHeader->Size, pData);
	}
}

void Session::JoinLobby()
{
	JoinRoomByRoomID(DEFAULT_ROOM_ID);
}

void Session::ExitRoom()
{
	mpRoom->UserExit(this);
	if (mpRoom->GetCurPeopleCount() == 0)
	{
		mpServer->DestroyRoom(mpRoom);
		mpRoom = nullptr;
	}
}

void Session::CreateRoom(const char* pTitle, int maxCount)
{
	if (JoinRoomByRoom(mpServer->CreateRoom(pTitle, maxCount)))
	{
		mpState->ChangeState(eSessionState::Room);
	}
}

void Session::SendRoomInfo()
{
	std::vector<Room*> roomList = mpServer->GetRoomList();

	if (roomList.empty())
	{
		MsgRoomInfo msg;
		msg.Length = 0;
		PostSend(false, msg.Size, (char*)&msg);
		return;
	}

	MsgRoomInfo msg;
	msg.Length = roomList.size();
	
	for (int i = 0; i < roomList.size(); ++i)
	{
		msg.Index = i;
		sprintf_s(msg.Title, MAX_NAME_LEN - 1, "%s", roomList[i]->GetTitle());
		msg.CurCount = roomList[i]->GetCurPeopleCount();
		msg.MaxCount = roomList[i]->GetMaxPeopleCount();
		msg.RoomID = roomList[i]->GetRoomID();
		PostSend(false, msg.Size, (char*)&msg);
	}
}

bool Session::JoinRoomByRoomID(room_id roomID)
{
	return JoinRoomByRoom(mpServer->GetRoom(roomID));
}

bool Session::JoinRoomByRoom(Room* pRoom)
{
	if (pRoom == nullptr || pRoom->IsFull())
	{
		MsgRoomJoinFail msg;
		PostSend(false, msg.Size, (char*)&msg);
		return false;
	}
	pRoom->UserJoin(this);
	if (mpRoom != nullptr)
	{
		ExitRoom();
	}
	mpRoom = pRoom;
	MsgRoomJoinSuccess msg;
	sprintf_s(msg.Title, MAX_NAME_LEN, "%s", pRoom->GetTitle());
	msg.CurCount = pRoom->GetCurPeopleCount();
	msg.MaxCount = pRoom->GetMaxPeopleCount();
	PostSend(false, msg.Size, (char*)&msg);
	return true;
}

void Session::HandleReceive(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (error)
	{
		PrintErrorCode(error);
		mpServer->CloseSession(mUserID);
		return;
	}

	memcpy(&mPacketBuffer[mPacketBufferMark], mReceiveBuffer, bytes_transferred);

	int packetData = mPacketBufferMark + bytes_transferred;

	int readData = 0;

	while (packetData > 0)
	{
		if (packetData < sizeof(Message))
		{
			break;
		}

		Message* pHeader = (Message*)&mPacketBuffer[readData];

		if (pHeader->Size <= packetData)
		{
			ProcessPacket((char*)&mPacketBuffer[readData]);

			packetData -= pHeader->Size;
			readData += pHeader->Size;
		}
		else
		{
			break;
		}
	}

	if (packetData > 0)
	{
		char tempBuffer[MAX_RECEIVE_BUFFER_LEN] = { 0, };
		memcpy(&tempBuffer[0], &mPacketBuffer[readData], packetData);
		memcpy(&mPacketBuffer[0], &tempBuffer[0], packetData);
	}

	mPacketBufferMark = packetData;

	PostReceive();
}

void Session::ProcessPacket(char* pData)
{
	Message* pHeader = (Message*)pData;
	switch (pHeader->Tag)
	{
	case eMessageTag::SetName:
	{
		MsgSetName* packet = (MsgSetName*)pData;
		sprintf_s(mName, MAX_NAME_LEN - 1, "%s", packet->Name);
		std::cout << "ÀÌ¸§ : " << mName << std::endl;
	}
	break;
	case eMessageTag::RoomText:
		mpRoom->ProcessMessage(mUserID, pData);
		break;
	}

	mpState->Process(pHeader);
}