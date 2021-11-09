#include "stdafx.h"
#include "Server.h"
#include "Session.h"
#include "Room.h"

Server::Server(boost::asio::io_service& ioService)
	:mAcceptor { ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER) }
{
	mbIsAccepting = false;
	for (int i = 0; i < MAX_USER_COUNT; ++i)
	{
		Session* pSession = new Session(i, ioService, this);
		mVecSession.push_back(pSession);
		mQueUserID.push_back(i);
	}

	Room* pRoom = new Room(this, MAX_USER_COUNT);
	pRoom->SetTitle("채팅방");
	mVecRoom.push_back(pRoom);
}

Server::~Server()
{
	for (size_t i = 0; i < mVecSession.size(); ++i)
	{
		if (mVecSession[i]->GetSocket().is_open())
		{
			mVecSession[i]->GetSocket().close();
		}

		delete mVecSession[i];
	}
}

void Server::Start()
{
	std::cout << "서버 시작" << std::endl;

	PostAccept();
}

void Server::CloseSession(const user_id userID)
{
	mVecSession[userID]->GetRoom()->UserExit(mVecSession[userID]);
}

void Server::ProcessPacket(const user_id userID, const char* pData)
{

}

bool Server::PostAccept()
{
	if (mQueUserID.empty())
	{
		mbIsAccepting = false;
		return false;
	}

	mbIsAccepting = true;
	user_id userID = mQueUserID.front();

	mQueUserID.pop_front();

	mAcceptor.async_accept(mVecSession[userID]->GetSocket(),
		boost::bind(&Server::HandleAccept, this,
			mVecSession[userID],
			boost::asio::placeholders::error)
	);

	return true;
}

void Server::HandleAccept(Session* pSession, const boost::system::error_code& error)
{
	if (error)
	{
		std::cout << "클라이언트 접속 실패" << std::endl;
		PrintErrorCode(error);
		return;
	}

	std::cout << "클라이언트 접속 성공 UserID : " << pSession->GetUserID() << std::endl;
	pSession->Init();
	pSession->PostReceive();
	MsgSetUserID msgUserID;
	msgUserID.UserID = pSession->GetUserID();
	pSession->PostSend(false, msgUserID.Size, (char*)&msgUserID);
	mVecRoom[DEFAULT_ROOM_ID]->UserJoin(pSession);
	pSession->JoinedRoom(mVecRoom[DEFAULT_ROOM_ID], DEFAULT_ROOM_ID);

	PostAccept();
}
