#pragma once
#include "Singleton.h"
#include "ObserverPattern.h"

class Client;
class NetworkManager : public Singleton<NetworkManager>, public IObserver
{
private:
	Client* mpClient = nullptr;

	bool mbIsReceiving = false;
	bool mbIsSending = false;
	std::queue<Message*> mQueMessage;
public:
	void Init(boost::asio::io_service& IOService);
	void Release();
	void Update();

	bool IsSending() { return mbIsSending; }

	void SetPlayerName(const char* pPlayerName);

	void SendChat(const char* pChat);
	void CreateRoom(const char* pTitle, int maxCount);
	void RefreshRoomInfo();
	void RequestJoinRoom(room_id roomID);

	void ReceiveMessage(Message* pMsg);
	void ReceiveEnd();

	virtual void Notice(Message* pMsg) override;
};