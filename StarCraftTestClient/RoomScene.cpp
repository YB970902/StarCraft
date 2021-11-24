#include "stdafx.h"
#include "RoomScene.h"

void RoomScene::Init()
{
	mChatIndex = 0;
}

void RoomScene::Release()
{
	mListChatContent.clear();
	memset(mChat, '\0', MAX_TEXT_LEN);
}

void RoomScene::Update()
{
	if (_kbhit())
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
		case 255:
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

void RoomScene::Render()
{
	system("cls");
	std::cout << "게임시작[F1] 방 나가기[ESC]\n";
	std::cout << "방 이름 [ " << USER->GetRoomName() << " ] ( " << USER->GetCurrentRoomCount() << " / " << USER->GetMaxRoomCount() << " )\n";
	std::cout << "이름 [" << USER->GetPlayerName() << "]\n";
	for (auto it = mListChatContent.begin(); it != mListChatContent.end(); ++it)
	{
		std::cout << "[" << it->first << "] [" << it->second << "]\n";
	}
	std::cout << "[" << mChat << "]\n";
}

void RoomScene::Notice(Message* pMsg)
{
	switch (pMsg->Tag)
	{
	case eMessageTag::RoomText:
		AddChat(((MsgRoomText*)pMsg)->Name, ((MsgRoomText*)pMsg)->Text);
		break;
	case eMessageTag::RoomExit:
		AddChat(((MsgRoomExit*)pMsg)->Name, "접속종료");
		break;
	}
}

void RoomScene::AddChat(std::string name, std::string chat)
{
	mListChatContent.emplace_back(name, chat);
	if (mListChatContent.size() > MAX_TEXT_LOG_SIZE) mListChatContent.pop_front();
}
