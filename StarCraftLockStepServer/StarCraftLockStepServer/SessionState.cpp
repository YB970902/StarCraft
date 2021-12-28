#include "stdafx.h"
#include "SessionState.h"
#include "Session.h"

SessionState::SessionState(Session* pSession)
	:mpSession{ pSession }
{

}

void SessionState::Init()
{
	AddState(eSessionState::Connecting, new ConnectingState(mpSession));
	AddState(eSessionState::Lobby, new LobbyState(mpSession));
	AddState(eSessionState::Room, new RoomState(mpSession));
	AddState(eSessionState::InGame, new InGameState(mpSession));
}

void SessionState::Release()
{
}

void SessionState::AddState(eSessionState tag, IState* pState)
{
	if (mpCurState == nullptr)
	{
		mpCurState = pState;
		mpCurState->Enter();
	}
	mMapState[tag] = pState;
}

void SessionState::ChangeState(eSessionState tag)
{
	mpCurState->Exit();
	mpCurState = mMapState[tag];
	mpCurState->Enter();
}

void SessionState::Process(Message* pMsg)
{
	if (mpCurState) { mpCurState->ProcessMessage(pMsg); }
}

void ConnectingState::Enter()
{
	MsgSetUserID msg;
	msg.UserID = mpSession->GetUserID();

	mpSession->PostSend(false, msg.Size, (char*)&msg);
}

void ConnectingState::Exit()
{
}

void ConnectingState::ProcessMessage(Message* pMsg)
{
	switch (pMsg->Tag)
	{
	case eMessageTag::SetName:
		mpSession->SetName(static_cast<MsgSetName*>(pMsg)->Name);
		mpSession->GetSessionState()->ChangeState(eSessionState::Lobby);
		break;
	}
}

void LobbyState::Enter()
{
	mpSession->JoinLobby();
}

void LobbyState::Exit()
{
}

void LobbyState::ProcessMessage(Message* pMsg)
{
	switch (pMsg->Tag)
	{
	case eMessageTag::ReqRoomInfo:
		mpSession->SendRoomInfo();
		break;
	case eMessageTag::RoomJoinRequest:
		if (mpSession->JoinRoomByRoomID(static_cast<MsgRoomJoinRequest*>(pMsg)->RoomID))
		{
			mpSession->GetSessionState()->ChangeState(eSessionState::Room);
		}
		break;
	case eMessageTag::RoomCreate:
		mpSession->CreateRoom(static_cast<MsgRoomCreate*>(pMsg)->Title, static_cast<MsgRoomCreate*>(pMsg)->MaxCount);
		break;
	}
}

void RoomState::Enter()
{
}

void RoomState::Exit()
{
}

void RoomState::ProcessMessage(Message* pMsg)
{
	switch (pMsg->Tag)
	{
	case eMessageTag::RoomExitRequest:
		mpSession->ExitRoom();
		break;
	case eMessageTag::RoomJoinRequest:
		mpSession->GetSessionState()->ChangeState(eSessionState::Lobby);
		break;
	}
}

void InGameState::Enter()
{
}

void InGameState::Exit()
{
}

void InGameState::ProcessMessage(Message* pMsg)
{
}
