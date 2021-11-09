#include "stdafx.h"
#include "Session.h"
#include "Server.h"
#include "Room.h"

Session::Session(user_id userID, boost::asio::io_service& IOService, Server* pServer)
	: mUserID{ userID }, mSocket{ IOService }, mpServer{ pServer }
{
	mPacketBufferMark = 0;
}

Session::~Session()
{
	while (mQueSendData.empty() == false)
	{
		delete[] mQueSendData.front();
		mQueSendData.pop_front();
	}
}

void Session::Init()
{

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

void Session::JoinedRoom(Room* pRoom, room_id ID)
{
	mpRoom = pRoom;
	MsgRoomJoinSuccess msgJoinSuccess;
	sprintf_s(msgJoinSuccess.Title, MAX_NAME_LEN - 1, "%s", pRoom->GetTitle());
	msgJoinSuccess.CurCount = pRoom->GetCurPeopleCount();
	msgJoinSuccess.MaxCount = pRoom->GetMaxPeopleCount();
	PostSend(false, msgJoinSuccess.Size, (char*)&msgJoinSuccess);
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
}