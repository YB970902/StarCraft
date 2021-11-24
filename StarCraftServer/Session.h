#pragma once

class Room;
class Server;
class SessionState;
class Session
{
protected:
	boost::asio::ip::tcp::socket mSocket;
	Server* mpServer = nullptr;
	Room* mpRoom = nullptr;
	SessionState* mpState = nullptr;

	char mReceiveBuffer[MAX_RECEIVE_BUFFER_LEN] = { 0, };
	int mPacketBufferMark = 0;
	char mPacketBuffer[MAX_RECEIVE_BUFFER_LEN * 2] = { 0, };

	std::deque<char*> mQueSendData;

	user_id mUserID = DEFAULT_USER_ID;
	char mName[MAX_NAME_LEN] = { 0, };
	bool mbIsJoinedRoom = false;
public:
	Session(user_id userID, boost::asio::io_service& IOService, Server* pServer);
	~Session();

	void Init();
	void Release();

	void PostSend(bool bIsImmediately, size_t size, char* pData);
	void PostReceive();

	void JoinLobby();
	bool JoinRoomByRoomID(room_id roomID);
	bool JoinRoomByRoom(Room* pRoom);
	void ExitRoom();

	void SendRoomInfo();
	void CreateRoom(const char* pTitle, int maxCount);

	inline boost::asio::ip::tcp::socket& GetSocket() { return mSocket; }
	inline Room* GetRoom() { return mpRoom; }
	inline void SetRoom(Room* pRoom) { mpRoom = pRoom; }
	inline SessionState* GetSessionState() { return mpState; }
	inline const user_id& GetUserID() { return mUserID; }
	inline const char* GetName() { return mName; }
	inline void SetName(const char* pName) { sprintf_s(mName, MAX_NAME_LEN, "%s", pName); }
protected:
	void HandleWrite(const boost::system::error_code& error, size_t bytes_transferred);
	void HandleReceive(const boost::system::error_code& error, size_t bytes_transferred);

	void ProcessPacket(char* pData);
};