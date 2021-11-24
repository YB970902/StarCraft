#include "stdafx.h"
#include "LoginScene.h"

void LoginScene::Init()
{
	memset(mName, '\0', MAX_NAME_LEN);
	mNameIndex = 0;
	mbIsChoicedName = false;
	mbIsGetID = false;

	mbIsPrintNotice = false;
	mElapsedNoticeTime = 0.0f;
}

void LoginScene::Release()
{

}

void LoginScene::Update()
{
	if (_kbhit() && !mbIsChoicedName)
	{
		char ch = _getch();
		switch (ch)
		{
		case VK_BACK:
			if (mNameIndex > 0)
			{
				mName[--mNameIndex] = '\0';
			}
			break;
		case VK_RETURN:
			if (mNameIndex == 0)
			{
				mbIsPrintNotice = true;
				mElapsedNoticeTime = 0.0f;
				mNoticeText = "[이름을 입력해주세요]\n";
			}
			else if (!mbIsGetID)
			{
				mbIsPrintNotice = true;
				mElapsedNoticeTime = 0.0f;
				mNoticeText = "[잠시후 다시시도해주세요]\n";
			}
			else
			{
				mbIsChoicedName = true;
				USER->SetPlayerName(mName);
				NET->SetPlayerName(mName);
			}
			break;
		case 255:
		default:
			if (mNameIndex < MAX_NAME_LEN - 1)
			{
				mName[mNameIndex] = ch;
				++mNameIndex;
			}
			break;
		}
	}

	if (mbIsPrintNotice)
	{
		mElapsedNoticeTime += DELTA_TIME;
		if (mElapsedNoticeTime >= mMaxNoticeTime) { mbIsPrintNotice = false; }
	}
}

void LoginScene::Render()
{
	system("cls");
	if (mbIsPrintNotice) { std::cout << mNoticeText; }
	std::cout << "이름 [" << mName << "]";
}

void LoginScene::Notice(Message* pMsg)
{
	switch (pMsg->Tag)
	{
	case eMessageTag::SetUserID:
		USER->SetPlayerID(static_cast<MsgSetUserID*>(pMsg)->UserID);
		mbIsGetID = true;
		break;
	case eMessageTag::RoomJoinSuccess:
		USER->SetCurrentRoomCount(static_cast<MsgRoomJoinSuccess*>(pMsg)->CurCount);
		USER->SetMaxRoomCount(static_cast<MsgRoomJoinSuccess*>(pMsg)->MaxCount);
		USER->SetRoomName(static_cast<MsgRoomJoinSuccess*>(pMsg)->Title);
		SCENE->ChangeScene(eSceneTag::Lobby);
		break;
	break;
	}
}
