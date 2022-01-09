#include "stdafx.h"
#include "MatchingScene.h"
#include "NetworkManager.h"
#include "TextGizmo.h"

void MatchingScene::Enter()
{
	mpText = static_cast<TextGizmo*>(UI->CreateText(Vector2(100, 100), Vector2(200, 100), 0, 1, 14, D2D1::ColorF::White, eTextAlign::Left));
}

void MatchingScene::Exit()
{
	UI->RemoveUI(mpText);
	mpText = nullptr;
}

void MatchingScene::Update()
{
	NET->Update();

	if (INPUT->IsOnceKeyDown(VK_ESCAPE))
	{
		NET->Release();
		SCENE->ChangeScene(eSceneTag::MainScene);
		return;
	}
}

void MatchingScene::ReceiveMessage(Message* pMsg)
{
	switch (pMsg->Tag)
	{
	case eMessageTag::SetUserID:
		NET->SetPlayerID(static_cast<MsgSetUserID*>(pMsg)->UserID);
		break;
	case eMessageTag::RoomJoinSuccess:
		mpText->SetText(TEXT("대기중"));
		if (static_cast<MsgRoomJoinSuccess*>(pMsg)->CurCount == static_cast<MsgRoomJoinSuccess*>(pMsg)->MaxCount)
		{
			mpText->SetText(TEXT("준비완료"));
		}
		break;
	case eMessageTag::RoomJoin:
		mpText->SetText(TEXT("준비완료"));
		break;
	}
}
