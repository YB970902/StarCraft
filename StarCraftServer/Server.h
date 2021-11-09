#pragma once

class Session;
class Room;
class Server
{
protected:
	bool mbIsAccepting = false;

	boost::asio::ip::tcp::acceptor mAcceptor;

	std::vector<Session*> mVecSession;
	std::vector<Room*> mVecRoom;
	std::deque<user_id> mQueUserID;

public:
	Server(boost::asio::io_service& ioService);
	~Server();

	void Start();
	void CloseSession(const user_id userID);
	void ProcessPacket(const user_id userID, const char* pData);
protected:
	bool PostAccept();
	void HandleAccept(Session* pSession, const boost::system::error_code& error);
};