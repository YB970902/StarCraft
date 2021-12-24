#include "stdafx.h"
#include "ChattingScene.h"
#include "UIManager.h"
#include "NetworkManager.h"
#include "TextGizmo.h"

void ChattingScene::Enter()
{
	UI->Init();
	NET->Init();

	mpText = static_cast<TextGizmo*>(UI->CreateText(Vector2(200, 200), Vector2(200, 200), 0, 1, 14, D2D1::ColorF::White));
	mpNotice = static_cast<TextGizmo*>(UI->CreateText(Vector2(200, 400), Vector2(200, 200), 0, 1, 14, D2D1::ColorF::White));

	memset(mName, '\0', MAX_NAME_LEN);
	mNameIndex = 0;
	mbIsChoicedName = false;
	mbIsGetID = false;

	mbIsPrintNotice = false;
	mElapsedNoticeTime = 0.0f;

	mpText->SetText(TEXT("이름 [  ]"));
}

void ChattingScene::Exit()
{
	NET->Release();
	UI->Release();
}

void ChattingScene::Update()
{
	if (INPUT->IsOnceKeyDown(VK_BACK))
	{
		if (mNameIndex > 0)
		{
			mName[--mNameIndex] = '\0';
			wstring tt;
			tt.append(TEXT("이름 [ "));
			tt.append(mName);
			tt.append(TEXT(" ]"));
			mpText->SetText(tt);
		}
	}
	if (INPUT->IsOnceKeyDown(VK_RETURN))
	{
		if (mNameIndex == 0)
		{
			mbIsPrintNotice = true;
			mElapsedNoticeTime = 0.0f;
			mpNotice->SetText(TEXT("[이름을 입력해주세요]"));
		}
		else if (!mbIsGetID)
		{
			mbIsPrintNotice = true;
			mElapsedNoticeTime = 0.0f;
			mpNotice->SetText(TEXT("[잠시후 다시시도해주세요]"));
		}
		else
		{
			mbIsChoicedName = true;
			NET->SetPlayerName(mName);
		}
	}

	wstring str = INPUT->GetTypedString();
	if (str.length() > 0)
	{
		for (int i = 0; i < str.length(); ++i)
		{
			if (mNameIndex >= MAX_NAME_LEN - 1)
			{
				break;
			}
			mName[mNameIndex] = str[i];
			++mNameIndex;
			wstring tt;
			tt.append(TEXT("이름 [ "));
			tt.append(mName);
			tt.append(TEXT(" ]"));
			mpText->SetText(tt);
		}
	}

	if (mbIsPrintNotice)
	{
		mElapsedNoticeTime += (float)DELTA_TIME;
		if (mElapsedNoticeTime >= mMaxNoticeTime)
		{
			mbIsPrintNotice = false;
			mpNotice->SetText(TEXT(""));
		}
	}

	if (INPUT->IsOnceKeyDown('Q'))
	{
		SCENE->ChangeScene(eSceneTag::MainScene);
		return;
	}

	UI->Update();
}

void ChattingScene::ReceiveMessage(Message* pMsg)
{
	switch (pMsg->Tag)
	{
	case eMessageTag::SetUserID:
		NET->SetPlayerID(static_cast<MsgSetUserID*>(pMsg)->UserID);
		mbIsGetID = true;
		break;
	case eMessageTag::RoomJoinSuccess:
		NET->SetCurrentRoomCount(static_cast<MsgRoomJoinSuccess*>(pMsg)->CurCount);
		NET->SetMaxRoomCount(static_cast<MsgRoomJoinSuccess*>(pMsg)->MaxCount);
		NET->SetRoomName(static_cast<MsgRoomJoinSuccess*>(pMsg)->Title);
		//SCENE->ChangeScene(eSceneTag::Lobby);
		break;
	}
}
