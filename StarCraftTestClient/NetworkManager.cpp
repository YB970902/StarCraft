#include "stdafx.h"
#include "NetworkManager.h"
#include "Client.h"

void NetworkManager::Init(boost::asio::io_service& IOService)
{
	mpClient = new Client(IOService);
	mpClient->Connect();
}

void NetworkManager::Release()
{
	mpClient->Close();
	delete mpClient;
	mpClient = nullptr;
}

void NetworkManager::Update()
{
	if (mQueMessage.empty() || mbIsReceiving) return;

	mbIsSending = true;

	Message* pMsg = nullptr;

	while (mQueMessage.empty() == false)
	{
		pMsg = mQueMessage.front();
		mQueMessage.pop();
		Notice(pMsg);
		// 유닛 매니저에 보내기
		SCENE->Notice(pMsg);
		delete pMsg;
		pMsg = nullptr;
	}

	mbIsSending = false;
}

void NetworkManager::SetPlayerName(const wchar_t* pPlayerName)
{
	MsgSetName msg;
	swprintf_s(msg.Name, MAX_NAME_LEN, TEXT("%s"), pPlayerName);
	mpClient->PostSend(false, sizeof(msg), (char*)&msg);
}

void NetworkManager::SendChat(const wchar_t* pChat)
{
	MsgRoomText msg;
	swprintf_s(msg.Name, MAX_NAME_LEN, TEXT("%s"), USER->GetPlayerName());
	swprintf_s(msg.Text, MAX_TEXT_LEN, TEXT("%s"), pChat);

	mpClient->PostSend(false, msg.Size, (char*)&msg);
}

void NetworkManager::CreateRoom(const wchar_t* pTitle, int maxCount)
{
	MsgRoomCreate msg;
	swprintf_s(msg.Title, MAX_NAME_LEN, TEXT("%s"), pTitle);
	msg.MaxCount = maxCount;

	mpClient->PostSend(false, msg.Size, (char*)&msg);
}

void NetworkManager::RefreshRoomInfo()
{
	MsgReqRoomInfo msg;
	mpClient->PostSend(false, msg.Size, (char*)&msg);
}

void NetworkManager::RequestJoinRoom(room_id roomID)
{
	MsgRoomJoinRequest msg;
	msg.RoomID = roomID;
	mpClient->PostSend(false, msg.Size, (char*)&msg);
}

void NetworkManager::ReceiveMessage(Message* pMsg)
{
	mbIsReceiving = true;

	Message* newMsg = nullptr;
	switch (pMsg->Tag)
	{
	case eMessageTag::SetUserID:
		newMsg = new MsgSetUserID();
		memcpy(newMsg, pMsg, sizeof(MsgSetUserID));
		break;
	case eMessageTag::RoomJoinFail:
		newMsg = new MsgRoomJoinFail();
		memcpy(newMsg, pMsg, sizeof(MsgRoomJoinFail));
		break;
	case eMessageTag::RoomJoinSuccess:
		newMsg = new MsgRoomJoinSuccess();
		memcpy(newMsg, pMsg, sizeof(MsgRoomJoinSuccess));
		break;
	case eMessageTag::RoomJoin:
		newMsg = new MsgRoomJoin();
		memcpy(newMsg, pMsg, sizeof(MsgRoomJoin));
		break;
	case eMessageTag::RoomExit:
		newMsg = new MsgRoomExit();
		memcpy(newMsg, pMsg, sizeof(MsgRoomExit));
		break;
	case eMessageTag::RoomInfo:
		newMsg = new MsgRoomInfo();
		memcpy(newMsg, pMsg, sizeof(MsgRoomInfo));
		break;
	case eMessageTag::RoomText:
		newMsg = new MsgRoomText();
		memcpy(newMsg, pMsg, sizeof(MsgRoomText));
		break;
	default:
		return;
	}
	mQueMessage.push(newMsg);
}

void NetworkManager::ReceiveEnd()
{
	mbIsReceiving = false;
}

void NetworkManager::Notice(Message* pMsg)
{
	switch (pMsg->Tag)
	{
	case eMessageTag::RoomJoinSuccess:
		USER->SetCurrentRoomCount(static_cast<MsgRoomJoinSuccess*>(pMsg)->CurCount);
		USER->SetMaxRoomCount(static_cast<MsgRoomJoinSuccess*>(pMsg)->MaxCount);
		USER->SetRoomName(static_cast<MsgRoomJoinSuccess*>(pMsg)->Title);
		break;
	case eMessageTag::RoomJoin:
		USER->SetCurrentRoomCount(USER->GetCurrentRoomCount() + 1);
		break;
	case eMessageTag::RoomExit:
		USER->SetCurrentRoomCount(USER->GetCurrentRoomCount() - 1);
		break;
	}
}
