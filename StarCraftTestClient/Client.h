#pragma once
#include <boost/thread.hpp>

class Client
{
protected:
	boost::asio::ip::tcp::endpoint mEndPoint;
	boost::asio::ip::tcp::socket mSocket;

	CRITICAL_SECTION mLock;

	char mReceiveBuffer[MAX_RECEIVE_BUFFER_LEN * 2] = { 0, };

	std::deque<char*> mQueSendData;

	int mPacketBufferMark = 0;
	char mPacketBuffer[MAX_RECEIVE_BUFFER_LEN * 2] = { 0, };

	int mCurRoomNumber = DEFAULT_ROOM_ID;
	bool mbIsJoinRoom = false;

	char mName[MAX_NAME_LEN] = { 0, };
	user_id mUserID = NONE_USER_ID;
	room_id mRoomID = DEFAULT_ROOM_ID;

public:
	Client(boost::asio::io_service& ioService);
	~Client();

	void Connect();
	void Close();

	void PostSend(const bool bIsImmediately, size_t size, char* pData);

	inline bool IsConnected() { return mSocket.is_open(); }
	inline bool IsJoinRoom() { return mbIsJoinRoom; }

	inline void SetName(const char* pName) { sprintf_s(mName, MAX_NAME_LEN - 1, "%s", pName); }
	inline const char* GetName() { return mName; }
	inline user_id GetUserID() { return mUserID; }

protected:
	void PostReceive();

	void HandleConnect(const boost::system::error_code& error);
	void HandleWrite(const boost::system::error_code& error, size_t bytes_transferred);
	void HandleRecieve(const boost::system::error_code& error, size_t bytes_transferred);
	void ProcessPacket(const char* pData);
};