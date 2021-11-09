#pragma once

class Room;
class Server;
class Session
{
protected:
	Room* mpRoom = nullptr;

	boost::asio::ip::tcp::socket mSocket;
	Server* mpServer = nullptr;

	char mReceiveBuffer[MAX_RECEIVE_BUFFER_LEN] = { 0, };
	int mPacketBufferMark = 0;
	char mPacketBuffer[MAX_RECEIVE_BUFFER_LEN * 2] = { 0, };

	std::deque<char*> mQueSendData;

	user_id mUserID = NONE_USER_ID;
	char mName[MAX_NAME_LEN] = { 0, };
	bool mbIsJoinedRoom = false;
public:
	Session(user_id userID, boost::asio::io_service& IOService, Server* pServer);
	~Session();

	void Init();

	void PostSend(bool bIsImmediately, size_t size, char* pData);
	void PostReceive();

	void JoinedRoom(Room* pRoom, room_id ID);

	inline boost::asio::ip::tcp::socket& GetSocket() { return mSocket; }
	inline void SetRoom(Room* pRoom) { mpRoom = pRoom; }
	inline Room* GetRoom() { return mpRoom; }
	inline const user_id& GetUserID() { return mUserID; }
	inline const char* GetName() { return mName; }
protected:
	void HandleWrite(const boost::system::error_code& error, size_t bytes_transferred);
	void HandleReceive(const boost::system::error_code& error, size_t bytes_transferred);

	void ProcessPacket(char* pData);
};