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
	std::cout << "�� ����[F1] ���ΰ�ħ[F5]\n";
	std::cout << "�κ� �� [ " << USER->GetRoomName() << " ] ( " << USER->GetCurrentRoomCount() << " / " << USER->GetMaxRoomCount() << " )\n";
	std::cout << "�̸� [" << USER->GetPlayerName() << "]\n\n";
	for (int i = 0; i < mVecRoom.size(); ++i)
	{
		std::cout << "�� ��ȣ [" << i << "] �� �̸�[" << mVecRoom[i].RoomName.c_str() << "] �ο� ( " << mVecRoom[i].CurCount << " / " << mVecRoom[i].MaxCount << " )\n";
	}
	std::cout << "\n";

	for (auto it = mListChatContent.begin(); it != mListChatContent.end(); ++it)
	{
		std::cout << "[" << it->Name.c_str() << "] [" << it->Content.c_str() << "]\n";
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
		AddChat(static_cast<MsgRoomExit*>(pMsg)->Name, TEXT("���� �κ񿡼� �������ϴ�."));
		break;
	case eMessageTag::RoomJoin:
		AddChat(static_cast<MsgRoomJoin*>(pMsg)->Name, TEXT("���� �κ� �����Ͽ����ϴ�."));
		break;
	case eMessageTag::RoomInfo:
		AddRoomInfo(static_cast<MsgRoomInfo*>(pMsg));
		break;
	case eMessageTag::RoomJoinFail:
		NET->RefreshRoomInfo();
		mbIsWaitingJoinRoom = false;
		mbIsShowNotice = true;
		mpNoticeText = TEXT("[�� �� ���� ���Դϴ�!]\n");
		break;
	case eMessageTag::RoomJoinSuccess:
		USER->SetRoomName(static_cast<MsgRoomJoinSuccess*>(pMsg)->Title);
		USER->SetCurrentRoomCount(static_cast<MsgRoomJoinSuccess*>(pMsg)->CurCount);
		USER->SetMaxRoomCount(static_cast<MsgRoomJoinSuccess*>(pMsg)->MaxCount);
		SCENE->ChangeScene(eSceneTag::Room);
		break;
	}
}

void LobbyScene::AddChat(std::wstring name, std::wstring chat)
{
	mListChatContent.emplace_back(ChatData(name, chat));
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
