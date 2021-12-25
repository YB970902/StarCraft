#include "stdafx.h"
#include "CreateRoomScene.h"

void CreateRoomScene::Init()
{
	mTitleIndex = 0;
	mMaxCount = 2;
	mbIsCreateRoom = false;
}

void CreateRoomScene::Release()
{
	memset(mTitle, '\0', MAX_NAME_LEN);
}

void CreateRoomScene::Update()
{
	if (mbIsCreateRoom) { return; }
	if (_kbhit())
	{
		char ch = _getch();
		switch (ch)
		{
		case VK_BACK:
			if (mTitleIndex > 0)
			{
				mTitle[--mTitleIndex] = '\0';
			}
			break;
		case VK_RETURN:
			if (mTitleIndex > 0)
			{
				mTitle[mTitleIndex] = '\0';
				NET->CreateRoom(mTitle, mMaxCount);
				mbIsCreateRoom = true;
			}
			break;
		default:
			if (mTitleIndex < MAX_NAME_LEN - 1)
			{
				mTitle[mTitleIndex] = ch;
				++mTitleIndex;
			}
			break;
		}
	}
}

void CreateRoomScene::Render()
{
	system("cls");
	std::wcout << "생성할 방의 이름 [" << mTitle << "]\n";
}

void CreateRoomScene::Notice(Message* pMsg)
{
	switch (pMsg->Tag)
	{
	case eMessageTag::RoomJoinSuccess:
		USER->SetRoomName(static_cast<MsgRoomJoinSuccess*>(pMsg)->Title);
		USER->SetMaxRoomCount(static_cast<MsgRoomJoinSuccess*>(pMsg)->MaxCount);
		USER->SetCurrentRoomCount(static_cast<MsgRoomJoinSuccess*>(pMsg)->CurCount);
		SCENE->ChangeScene(eSceneTag::Room);
		break;
	}
}
