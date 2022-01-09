#include "stdafx.h"
#include "ChattingScene.h"
#include "NetworkManager.h"
#include "TextGizmo.h"

void ChattingScene::Enter()
{
	mState[eChattingState::Login] = new LoginState();
	mState[eChattingState::Lobby] = new LobbyState();
	mState[eChattingState::CreateRoom] = new CreateRoomState();
	mState[eChattingState::Room] = new RoomState();

	for (auto it = mState.begin(); it != mState.end(); ++it)
	{
		it->second->Init(this);
	}
	ChangeState(eChattingState::Login);
}

void ChattingScene::Exit()
{
	if (mpCurState)
	{
		mpCurState->Exit();
		mpCurState = nullptr;
	}

	for (auto it = mState.begin(); it != mState.end();)
	{
		SAFE_DELETE(it->second);
		it = mState.erase(it);
	}
}

void ChattingScene::Update()
{
	if (mpCurState) { mpCurState->Update(); }

	if (INPUT->IsOnceKeyDown(VK_ESCAPE))
	{
		NET->Release();
		SCENE->ChangeScene(eSceneTag::MainScene);
		return;
	}

	UI->Update();
}

void ChattingScene::ChangeState(eChattingState stateTag)
{
	if (mpCurState) { mpCurState->Exit(); }
	mpCurState = mState[stateTag];
	mpCurState->Enter();
}

void ChattingScene::ReceiveMessage(Message* pMsg)
{
	if (mpCurState) { mpCurState->ReceiveMessage(pMsg); }
}

void LobbyState::Enter()
{
	mChatIndex = 0;
	mbIsWaitingJoinRoom = false;
	NET->RefreshRoomInfo();

	mpUI = static_cast<TextGizmo*>(UI->CreateText(Vector2(100, 100), Vector2(200, 400), 0, 1, 14, D2D1::ColorF::Yellow, eTextAlign::Left));
	mpText = static_cast<TextGizmo*>(UI->CreateText(Vector2(100, 400), Vector2(200, 100), 0, 1, 14, D2D1::ColorF::White, eTextAlign::Left));
}

void LobbyState::Exit()
{
	UI->RemoveUI(mpUI);
	UI->RemoveUI(mpText);
	mListChatContent.clear();
	memset(mChat, '\0', MAX_TEXT_LEN);

	mVecRoom.clear();
	mVecRoomReceiving.clear();
}

void LobbyState::Update()
{
	if (mbIsShowNotice)
	{
		mElapsedNoticeTime += DELTA_TIME;
		if (mElapsedNoticeTime > mMaxNoticeTime)
		{
			mbIsShowNotice = false;
		}
	}

	if (mbIsWaitingJoinRoom) { return; }

	if (INPUT->IsOnceKeyDown(VK_BACK))
	{
		if (mChatIndex > 0)
		{
			mChat[--mChatIndex] = '\0';
		}
	}
	if (INPUT->IsOnceKeyDown(VK_RETURN))
	{
		if (mChatIndex > 0)
		{
			mChat[mChatIndex] = '\0';

			AddChat(NET->GetPlayerName(), mChat);
			NET->SendChat(mChat);

			mChatIndex = 0;
			memset(mChat, '\0', MAX_TEXT_LEN);
		}
	}
	if (INPUT->IsOnceKeyDown(59)) // F1
	{
		mpScene->ChangeState(eChattingState::CreateRoom);
	}
	if (INPUT->IsOnceKeyDown(63)) // F5
	{
		NET->RefreshRoomInfo();
	}
	for (int i = 0; i < 10; ++i)
	{
		if (INPUT->IsOnceKeyDown(48 + i))
		{
			if (i >= 0 && i < mVecRoom.size())
			{
				mbIsWaitingJoinRoom = true;
				NET->RequestJoinRoom(mVecRoom[i].RoomID);
			}
		}
	}

	wstring str = INPUT->GetTypedString();
	if (str.length() > 0)
	{
		for (int i = 0; i < str.length(); ++i)
		{
			if (mChatIndex >= MAX_TEXT_LEN - 1) { break; }
			mChat[mChatIndex] = str[i];
			++mChatIndex;
		}
	}
}

void LobbyState::ReceiveMessage(Message* pMsg)
{
	switch (pMsg->Tag)
	{
	case eMessageTag::RoomText:
		AddChat(static_cast<MsgRoomText*>(pMsg)->Name, static_cast<MsgRoomText*>(pMsg)->Text);
		break;
	case eMessageTag::RoomExit:
		AddChat(static_cast<MsgRoomExit*>(pMsg)->Name, TEXT("님이 로비에서 나갔습니다."));
		break;
	case eMessageTag::RoomJoin:
		AddChat(static_cast<MsgRoomJoin*>(pMsg)->Name, TEXT("님이 로비에 접속하였습니다."));
		break;
	case eMessageTag::RoomInfo:
		AddRoomInfo(static_cast<MsgRoomInfo*>(pMsg));
		break;
	case eMessageTag::RoomJoinFail:
		NET->RefreshRoomInfo();
		mbIsWaitingJoinRoom = false;
		mbIsShowNotice = true;
		mpNoticeText = TEXT("[들어갈 수 없는 방입니다!]\n");
		break;
	case eMessageTag::RoomJoinSuccess:
		NET->SetRoomName(static_cast<MsgRoomJoinSuccess*>(pMsg)->Title);
		NET->SetCurrentRoomCount(static_cast<MsgRoomJoinSuccess*>(pMsg)->CurCount);
		NET->SetMaxRoomCount(static_cast<MsgRoomJoinSuccess*>(pMsg)->MaxCount);
		mpScene->ChangeState(eChattingState::Room);
		break;
	}
}

void LobbyState::AddChat(std::wstring name, std::wstring chat)
{
	mListChatContent.emplace_back(ChatData(name, chat));
	if (mListChatContent.size() > MAX_TEXT_LOG_SIZE) mListChatContent.pop_front();
}

void LobbyState::AddRoomInfo(MsgRoomInfo* pMsg)
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

void LobbyState::SetUIText()
{
	wstringstream wss;

	wss << TEXT("방 생성[F1] 새로고침[F5]\n");
	wss << TEXT("로비 명 [ ") << NET->GetRoomName() << TEXT(" ] ( ") << NET->GetCurrentRoomCount() << TEXT(" / ") << NET->GetMaxRoomCount() << TEXT(" )\n");
	wss << TEXT("이름 [") << NET->GetPlayerName() << TEXT("]\n\n");

	for (int i = 0; i < mVecRoom.size(); ++i)
	{
		wss << TEXT("방 번호 [") << i << TEXT("] 방 이름[") << mVecRoom[i].RoomName.c_str() << "] 인원 ( " << mVecRoom[i].CurCount << TEXT(" / ") << mVecRoom[i].MaxCount << TEXT(" )\n");
	}

	for (auto it = mListChatContent.begin(); it != mListChatContent.end(); ++it)
	{
		wss << TEXT("[") << it->Name.c_str() << TEXT("] [") << it->Content.c_str() << TEXT("]\n");
	}

	mpText->SetText(wss.str());
}

void LobbyState::SetText()
{
	wstringstream wss;

	wss << TEXT("[") << mChat << TEXT("]");

	mpText->SetText(wss.str());
}

void LoginState::Enter()
{
	mpText = static_cast<TextGizmo*>(UI->CreateText(Vector2(200, 200), Vector2(200, 200), 0, 1, 14, D2D1::ColorF::White, eTextAlign::Left));
	mpNotice = static_cast<TextGizmo*>(UI->CreateText(Vector2(200, 300), Vector2(200, 200), 0, 1, 14, D2D1::ColorF::Yellow, eTextAlign::Left));

	memset(mName, '\0', MAX_NAME_LEN);
	mNameIndex = 0;
	mbIsChoicedName = false;
	mbIsGetID = false;

	mbIsPrintNotice = false;
	mElapsedNoticeTime = 0.0f;

	mpText->SetText(TEXT("이름 [  ]"));
}

void LoginState::Exit()
{
	UI->RemoveUI(mpText);
	UI->RemoveUI(mpNotice);
}

void LoginState::Update()
{
	if (mbIsChoicedName) { return; }

	if (INPUT->IsOnceKeyDown(VK_BACK))
	{
		if (mNameIndex > 0)
		{
			mName[--mNameIndex] = '\0';
			SetName();
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
		else if (mbIsGetID)
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
			SetName();
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
}

void LoginState::ReceiveMessage(Message* pMsg)
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
		mpScene->ChangeState(eChattingState::Lobby);
		break;
	}
}

void LoginState::SetName()
{
	wstring text;
	text.append(TEXT("이름 [ "));
	text.append(mName);
	text.append(TEXT(" ]"));
	mpText->SetText(text);
}

void CreateRoomState::Enter()
{
}

void CreateRoomState::Exit()
{
}

void CreateRoomState::Update()
{
}

void CreateRoomState::ReceiveMessage(Message* pMsg)
{
}

void RoomState::Enter()
{
}

void RoomState::Exit()
{
}

void RoomState::Update()
{
}

void RoomState::ReceiveMessage(Message* pMsg)
{
}
