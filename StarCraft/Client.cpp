#include "stdafx.h"
#include "Client.h"
#include "NetworkManager.h"

Client::Client(boost::asio::io_service& ioService)
	:mEndPoint(boost::asio::ip::address::from_string(SERVER_ADDRESS), PORT_NUMBER),
	mSocket(ioService)
{
	mbIsReleased = false;
	mbIsJoinRoom = false;
	InitializeCriticalSectionAndSpinCount(&mLock, 4000);
}

Client::~Client()
{
	EnterCriticalSection(&mLock);

	while (mQueSendData.empty() == false)
	{
		delete[] mQueSendData.front();
		mQueSendData.pop_front();
	}

	LeaveCriticalSection(&mLock);

	DeleteCriticalSection(&mLock);
}

void Client::Connect()
{
	mPacketBufferMark = 0;

	mSocket.async_connect(mEndPoint,
		boost::bind(&Client::HandleConnect, this,
			boost::asio::placeholders::error)
	);
}

void Client::Close()
{
	EnterCriticalSection(&mLock);
	mbIsReleased = true;
	if (mSocket.is_open())
	{
		mSocket.close();
	}
	LeaveCriticalSection(&mLock);
}

void Client::PostSend(const bool bIsImmediately, size_t size, char* pData)
{
	char* pSendData = nullptr;

	EnterCriticalSection(&mLock);

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

	if (bIsImmediately || mQueSendData.size() < 2)
	{
		boost::asio::async_write(mSocket, boost::asio::buffer(pSendData, size),
			boost::bind(&Client::HandleWrite, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)
		);
	}

	LeaveCriticalSection(&mLock);
}

void Client::PostReceive()
{
	memset(&mReceiveBuffer, '\0', sizeof(mReceiveBuffer));

	mSocket.async_read_some
	(
		boost::asio::buffer(mReceiveBuffer),
		boost::bind(&Client::HandleReceive, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)
	);
}

void Client::HandleConnect(const boost::system::error_code& error)
{
	if (!error)
	{
		PostReceive();
	}
	else
	{
		PrintErrorCode(error);
	}
}

void Client::HandleWrite(const boost::system::error_code& error, size_t bytes_transferred)
{
	EnterCriticalSection(&mLock);

	delete[] mQueSendData.front();
	mQueSendData.pop_front();

	char* pData = nullptr;

	if (mQueSendData.empty() == false)
	{
		pData = mQueSendData.front();
	}

	LeaveCriticalSection(&mLock);

	if (pData)
	{
		Message* pMsg = (Message*)pData;
		PostSend(true, pMsg->Size, pData);
	}
}

void Client::HandleReceive(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (error)
	{
		PrintErrorCode(error);
		Close();
		return;
	}

	memcpy(&mPacketBuffer[mPacketBufferMark], mReceiveBuffer, bytes_transferred);

	int packetData = mPacketBufferMark + bytes_transferred;
	int readData = 0;

	while (packetData > 0)
	{
		if (NET->IsSending()) continue;

		if (packetData < sizeof(Message))
		{
			break;
		}

		Message* pMsg = (Message*)&mPacketBuffer[readData];
		if (pMsg->Size <= packetData)
		{
			EnterCriticalSection(&mLock);
			if (mbIsReleased) { return; }
			NET->ReceiveMessage(pMsg);

			packetData -= pMsg->Size;
			readData += pMsg->Size;
			LeaveCriticalSection(&mLock);
		}
		else
		{
			break;
		}
	}

	EnterCriticalSection(&mLock);
	if (mbIsReleased) { return; }

	if (packetData > 0)
	{
		char TempBuffer[MAX_RECEIVE_BUFFER_LEN] = { 0, };
		memcpy(&TempBuffer[0], &mPacketBuffer[readData], packetData);
		memcpy(&mPacketBuffer[0], &TempBuffer[0], packetData);
	}

	NET->ReceiveEnd();

	mPacketBufferMark = packetData;

	LeaveCriticalSection(&mLock);

	PostReceive();
}