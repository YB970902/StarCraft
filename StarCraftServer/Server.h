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
	std::deque<room_id> mQueRoomID;

public:
	Server(boost::asio::io_service& ioService);
	~Server();

	void Start();
	void CloseSession(const user_id userID);
	void ProcessPacket(const user_id userID, const char* pData);

	Room* CreateRoom(const char* pTitle, int maxCount);
	void DestroyRoom(Room* pRoom);
	Room* GetRoom(room_id roomID);
	std::vector<Room*> GetRoomList();
protected:
	bool PostAccept();
	void HandleAccept(Session* pSession, const boost::system::error_code& error);
};