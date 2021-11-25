#include "stdafx.h"
#include "LobbyScene.h"

void LobbyScene::Init()
{
	mChatIndex = 0;
	mbIsWaitingJoinRoom = false;
	NET->RefreshRoomInfo();
}

void LobbyScene::Release()
{
	mListChatContent.clear();
	memset(mChat, '\0', MAX_TEXT_LEN);

	mVecRoom.clear();
	mVecRoomReceiving.clear();
}

void LobbyScene::Update()
{
	if (mbIsShowNotice)
	{
		mElapsedNoticeTime += DELTA_TIME;
		if (mElapsedNoticeTime > mMaxNoticeTime)
		{
			mbIsShowNotice = false;
		}
	}
	if (_kbhit() && !mbIsWaitingJoinRoom)
	{
		char ch = _getch();
		switch (ch)
		{
		case VK_BACK:
			if (mChatIndex > 0)
			{
				mChat[--mChatIndex] = '\0';
			}
			break;
		case VK_RETURN:
			if (mChatIndex > 0)
			{
				mChat[mChatIndex] = '\0';

				AddChat(USER->GetPlayerName(), mChat);
				NET->SendChat(mChat);

				mChatIndex = 0;
				memset(mChat, '\0', MAX_TEXT_LEN);
			}
			break;
		case 59: // F1
			SCENE->ChangeScene(eSceneTag::CreateRoom);
			return;
		case 63: // F5
			NET->RefreshRoomInfo();
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		{
			int index = atoi((char*)&ch);
			if (index >= 0 && index < mVecRoom.size())
			{
				mbIsWaitingJoinRoom = true;
				NET->RequestJoinRoom(mVecRoom[index].RoomID);
			}
		}
		break;
		default:
			if (mChatIndex < MAX_TEXT_LEN - 1)
			{
				mChat[mChatIndex] = ch;
				++mChatIndex;
			}
			break;
		}
	}
}

void LobbyScene::Render()
{
	system("cls");
	if (mbIsShowNotice)
	{
		std::cout << mpNoticeText;
	}
	std::cout << "방 생성[F1] 새로고침[F5]\n";
	std::cout << "로비 명 [ " << USER->GetRoomName() << " ] ( " << USER->GetCurrentRoomCount() << " / " << USER->GetMaxRoomCount() << " )\n";
	std::cout << "이름 [" << USER->GetPlayerName() << "]\n\n";
	for (int i = 0; i < mVecRoom.size(); ++i)
	{
		std::cout << "방 번호 [" << i << "] 방 이름[" << mVecRoom[i].RoomName << "] 인원 ( " << mVecRoom[i].CurCount << " / " << mVecRoom[i].MaxCount << " )\n";
	}
	std::cout << "\n";

	for (auto it = mListChatContent.begin(); it != mListChatContent.end(); ++it)
	{
		std::cout << "[" << it->first << "] [" << it->second << "]\n";
	}
	std::cout << "[" << mChat << "]\n";
}

void LobbyScene::Notice(Message* pMsg)
{
	switch (pMsg->Tag)
	{
	case eMessageTag::RoomText:
		AddChat(static_cast<MsgRoomText*>(pMsg)->Name, static_cast<MsgRoomText*>(pMsg)->Text);
		break;
	case eMessageTag::RoomExit:
		AddChat(static_cast<MsgRoomExit*>(pMsg)->Name, "님이 로비에서 나갔습니다.");
		break;
	case eMessageTag::RoomJoin:
		AddChat(static_cast<MsgRoomJoin*>(pMsg)->Name, "님이 로비에 접속하였습니다.");
		break;
	case eMessageTag::RoomInfo:
		AddRoomInfo(static_cast<MsgRoomInfo*>(pMsg));
		break;
	case eMessageTag::RoomJoinFail:
		NET->RefreshRoomInfo();
		mbIsWaitingJoinRoom = false;
		mbIsShowNotice = true;
		mpNoticeText = "[들어갈 수 없는 방입니다!]\n";
		break;
	case eMessageTag::RoomJoinSuccess:
		USER->SetRoomName(static_cast<MsgRoomJoinSuccess*>(pMsg)->Title);
		USER->SetCurrentRoomCount(static_cast<MsgRoomJoinSuccess*>(pMsg)->CurCount);
		USER->SetMaxRoomCount(static_cast<MsgRoomJoinSuccess*>(pMsg)->MaxCount);
		SCENE->ChangeScene(eSceneTag::Room);
		break;
	}
}

void LobbyScene::AddChat(std::string name, std::string chat)
{
	mListChatContent.emplace_back(name, chat);
	if (mListChatContent.size() > MAX_TEXT_LOG_SIZE) mListChatContent.pop_front();
}

void LobbyScene::AddRoomInfo(MsgRoomInfo* pMsg)
{
	if (pMsg->Length == 0)
	{
		mVecRoom.clear();
		mVecRoomReceiving.clear();
		return;
	}

	if (mVecRoomReceiving.size() < pMsg->Length) { mVecRoomReceiving.resize(pMsg->Length); }
	++mReceivingCount;
	mVecRoomReceiving[pMsg->Index] = pMsg;
	if (mReceivingCount == pMsg->Length)
	{
		mReceivingCount = 0;
		mVecRoom = mVecRoomReceiving;
		mVecRoomReceiving.clear();
	}
}
