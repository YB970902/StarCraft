#include "stdafx.h"
#include "NetworkManager.h"
#include "RandomManager.h"
#include "UserManager.h"
#include "UnitManager.h"
#include "Protocol.h"
#include "Client.h"

void NetworkManager::Init()
{
	mbIsInit = true;
	mIOService = make_shared<boost::asio::io_service>();
	mpClient = new Client(*mIOService);
	mpClient->Connect();
	mThread = make_shared<boost::thread>(boost::bind(&boost::asio::io_service::run, mIOService));
}

void NetworkManager::Release()
{
	mbIsInit = false;
	mIOService.get()->stop();
	if (mpClient)
	{
		mpClient->Close();
		SAFE_DELETE(mpClient);
	}

	Message* pMsg = nullptr;

	while (mQueMessage.empty() == false)
	{
		pMsg = mQueMessage.front();
		mQueMessage.pop();
		SAFE_DELETE(pMsg);
	}

	mIOService = nullptr;
	mThread = nullptr;
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
		ProcessMessage(pMsg);
		// 유닛 매니저에 보내기
		SCENE->ReceiveMessage(pMsg);
		delete pMsg;
		pMsg = nullptr;
	}

	mbIsSending = false;
}

void NetworkManager::SetPlayerName(const wchar_t* pPlayerName)
{
	swprintf_s(mPlayerName, MAX_NAME_LEN - 1, TEXT("%s"), pPlayerName);
	MsgSetName msg;
	swprintf_s(msg.Name, MAX_NAME_LEN, TEXT("%s"), pPlayerName);
	mpClient->PostSend(false, sizeof(msg), (char*)&msg);
}

void NetworkManager::SendChat(const wchar_t* pChat)
{
	MsgRoomText msg;
	swprintf_s(msg.Name, MAX_NAME_LEN, TEXT("%s"), GetPlayerName());
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
	case eMessageTag::GameStart:
		newMsg = new MsgGameStart();
		memcpy(newMsg, pMsg, sizeof(MsgGameStart));
		break;
	case eMessageTag::InputData:
		newMsg = new MsgInputData();
		memcpy(newMsg, pMsg, sizeof(MsgInputData));
		break;
	case eMessageTag::UnitSpawn:
		newMsg = new MsgUnitSpawn();
		memcpy(newMsg, pMsg, sizeof(MsgUnitSpawn));
		break;
	case eMessageTag::UnitMove:
		newMsg = new MsgUnitMove();
		memcpy(newMsg, pMsg, sizeof(MsgUnitMove));
		break;
	case eMessageTag::UnitMoveAlertly:
		newMsg = new MsgUnitMoveAlertly();
		memcpy(newMsg, pMsg, sizeof(MsgUnitMoveAlertly));
		break;
	case eMessageTag::UnitAttack:
		newMsg = new MsgUnitAttack();
		memcpy(newMsg, pMsg, sizeof(MsgUnitAttack));
		break;
	case eMessageTag::UnitStop:
		newMsg = new MsgUnitStop();
		memcpy(newMsg, pMsg, sizeof(MsgUnitStop));
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

void NetworkManager::ProcessMessage(Message* pMsg)
{
	switch (pMsg->Tag)
	{
	case eMessageTag::RoomJoinSuccess:
		SetCurrentRoomCount(static_cast<MsgRoomJoinSuccess*>(pMsg)->CurCount);
		SetMaxRoomCount(static_cast<MsgRoomJoinSuccess*>(pMsg)->MaxCount);
		SetRoomName(static_cast<MsgRoomJoinSuccess*>(pMsg)->Title);
		break;
	case eMessageTag::RoomJoin:
		SetCurrentRoomCount(GetCurrentRoomCount() + 1);
		break;
	case eMessageTag::RoomExit:
		SetCurrentRoomCount(GetCurrentRoomCount() - 1);
		break;
	case eMessageTag::GameStart:
		RANDOM->SetSeed(static_cast<MsgGameStart*>(pMsg)->RandomSeed);
		USER->SetTeamTag((eTeamTag)static_cast<MsgGameStart*>(pMsg)->TeamTag);
		SCENE->ChangeScene(eSceneTag::MultiGameScene);
		break;
	case eMessageTag::InputData:
	{
		MsgInputData* pInputData = static_cast<MsgInputData*>(pMsg);
		mMapOtherTurn[pInputData->Turn] = CommandInfo();
		mMapOtherTurn[pInputData->Turn].Count = pInputData->Count;
		FindTempData(pInputData->Turn, mMapOtherTurn[pInputData->Turn]);
	}
		break;
	case eMessageTag::UnitSpawn:
	case eMessageTag::UnitMove:
	case eMessageTag::UnitMoveAlertly:
	case eMessageTag::UnitAttack:
	case eMessageTag::UnitStop:
		ProcessUnitMessage(pMsg);
		break;
	}
}

void NetworkManager::AddCommand(Command* pCmd)
{
	if (mbIsInit == false)
	{
		ExecuteCommand(pCmd);
		SAFE_DELETE(pCmd);
		return;
	}

	mVecCommand.push_back(pCmd);
}

void NetworkManager::InitGame()
{
	mCurSubTurn = 0;
	mCurTurn = -2;

	mMapTurn[0] = CommandInfo();
	mMapTurn[1] = CommandInfo();

	mMapOtherTurn[0] = CommandInfo();
	mMapOtherTurn[1] = CommandInfo();
}

void NetworkManager::ReleaseGame()
{
	// 다시보기 하려면 지우면 안됨
	for (auto it = mMapTurn.begin(); it != mMapTurn.end();)
	{
		for (int i = 0; i < it->second.Commands.size(); ++i)
		{
			delete (*it).second.Commands[i];
		}
		it->second.Commands.clear();
		it = mMapTurn.erase(it);
	}

	for (auto it = mMapOtherTurn.begin(); it != mMapOtherTurn.end();)
	{
		for (int i = 0; i < it->second.Commands.size(); ++i)
		{
			delete (*it).second.Commands[i];
		}
		it->second.Commands.clear();
		it = mMapOtherTurn.erase(it);
	}

	for (int i = 0; i < mVecCommand.size(); ++i)
	{
		delete mVecCommand[i];
	}
	mVecCommand.clear();
}

bool NetworkManager::UpdateGame()
{
	++mCurSubTurn;
	if (mCurSubTurn == MAX_SUB_TURN)
	{
		mCurSubTurn = 0;
		++mCurTurn;

		if (mCurTurn < 0) { return true; }

		// 턴이 비어있거나 메시지가 덜 온경우
		if (mMapOtherTurn.find(mCurTurn) == mMapOtherTurn.end() ||
			mMapOtherTurn[mCurTurn].Count > mMapOtherTurn[mCurTurn].Commands.size())
		{
			EraseAllCommand();
			mCurSubTurn = MAX_SUB_TURN - 1;
			--mCurTurn;
			return false;
		}

		SendCommand();

		for (int i = 0; i < mMapTurn[mCurTurn].Commands.size(); ++i)
		{
			ExecuteCommand(mMapTurn[mCurTurn].Commands[i]);
		}

		for (int i = 0; i < mMapOtherTurn[mCurTurn].Commands.size(); ++i)
		{
			ExecuteCommand(mMapOtherTurn[mCurTurn].Commands[i]);
		}

		return true;
	}

	return true;
}

void NetworkManager::ExecuteCommand(Command* pCmd)
{
	switch (pCmd->Tag)
	{
	case eCommandTag::Create:
	{
		CreateCommand* pCreateCmd = static_cast<CreateCommand*>(pCmd);
		UNIT->ExecuteCreateUnit(pCreateCmd->TeamTag, pCreateCmd->UnitTag, pCreateCmd->PosX, pCreateCmd->PosY);
	}
	break;
	case eCommandTag::AttackGround:
	{
		AttackGroundCommand* pAttackGroundCmd = static_cast<AttackGroundCommand*>(pCmd);
		UNIT->ExecuteAttackGround(pAttackGroundCmd->ID, pAttackGroundCmd->PosX, pAttackGroundCmd->PosY);
	}
	break;
	case eCommandTag::AttackUnit:
	{
		AttackUnitCommand* pAttackTargetCmd = static_cast<AttackUnitCommand*>(pCmd);
		UNIT->ExecuteAttackUnit(pAttackTargetCmd->ID, pAttackTargetCmd->TargetID);
	}
	break;
	case eCommandTag::Move:
	{
		MoveCommand* pMoveCmd = static_cast<MoveCommand*>(pCmd);
		UNIT->ExecuteMoveUnit(pMoveCmd->ID, pMoveCmd->PosX, pMoveCmd->PosY);
	}
	break;
	case eCommandTag::Stop:
	{
		StopCommand* pStopCmd = static_cast<StopCommand*>(pCmd);
		UNIT->ExecuteStopUnit(pStopCmd->ID);
	}
	break;
	}
}

void NetworkManager::FindTempData(int turn, CommandInfo& info)
{
	for (auto it = mVecTempCommand.begin(); it != mVecTempCommand.end();)
	{
		if ((*it)->Turn == turn)
		{
			info.Commands.push_back(*it);
			it = mVecTempCommand.erase(it);
		}
		else
		{
			++it;
		}
	}
}

Command* NetworkManager::MessageToCommand(Message* pMsg)
{
	Command* pCmd = nullptr;
	switch (pMsg->Tag)
	{
	case eMessageTag::UnitSpawn:
	{
		MsgUnitSpawn* pSpawn = static_cast<MsgUnitSpawn*>(pMsg);
		pCmd = new CreateCommand((eTeamTag)pSpawn->TeamTag, (eUnitTag)pSpawn->UnitTag, pSpawn->SpawnPosX, pSpawn->SpawnPosY);
		pCmd->Turn = pSpawn->Turn;
	}
		break;
	case eMessageTag::UnitMove:
	{
		MsgUnitMove* pMove = static_cast<MsgUnitMove*>(pMsg);
		pCmd = new MoveCommand(pMove->ID, POINT{ pMove->DestPosX, pMove->DestPosY });
		pCmd->Turn = pMove->Turn;
	}
		break;
	case eMessageTag::UnitMoveAlertly:
	{
		MsgUnitMoveAlertly* pMove = static_cast<MsgUnitMoveAlertly*>(pMsg);
		pCmd = new AttackGroundCommand(pMove->ID, POINT{ pMove->DestPosX, pMove->DestPosY });
		pCmd->Turn = pMove->Turn;
	}
		break;
	case eMessageTag::UnitAttack:
	{
		MsgUnitAttack* pAttack = static_cast<MsgUnitAttack*>(pMsg);
		pCmd = new AttackUnitCommand(pAttack->ID, pAttack->TargetID);
		pCmd->Turn = pAttack->Turn;
	}
		break;
	case eMessageTag::UnitStop:
	{
		MsgUnitStop* pStop = static_cast<MsgUnitStop*>(pMsg);
		pCmd = new StopCommand(pStop->ID);
		pCmd->Turn = pStop->Turn;
	}
		break;
	default:
		return nullptr;
	}

	return pCmd;
}

void NetworkManager::ProcessUnitMessage(Message* pMsg)
{
	Command* pCmd = MessageToCommand(pMsg);

	if (mMapOtherTurn.find(pCmd->Turn) == mMapOtherTurn.end())
	{
		mVecTempCommand.push_back(pCmd);
	}
	else
	{
		mMapOtherTurn[pCmd->Turn].Commands.push_back(pCmd);
	}
}

void NetworkManager::SendCommand()
{
	int turn = mCurTurn + 2;
	MsgInputData inputData;
	inputData.Turn = turn;
	inputData.Count = mVecCommand.size();
	mpClient->PostSend(false, inputData.Size, (char*)&inputData);
	for (int i = 0; i < mVecCommand.size(); ++i)
	{
		switch (mVecCommand[i]->Tag)
		{
		case eCommandTag::Create:
		{
			CreateCommand* pCmd = static_cast<CreateCommand*>(mVecCommand[i]);
			MsgUnitSpawn msg;
			msg.Turn = turn;
			msg.TeamTag = (int)pCmd->TeamTag;
			msg.UnitTag = (int)pCmd->UnitTag;
			msg.SpawnPosX = pCmd->PosX;
			msg.SpawnPosY = pCmd->PosY;
			msg.Turn = turn;
			mpClient->PostSend(false, msg.Size, (char*)&msg);
		}
		break;
		case eCommandTag::AttackGround:
		{
			AttackGroundCommand* pCmd = static_cast<AttackGroundCommand*>(mVecCommand[i]);
			MsgUnitMoveAlertly msg;
			msg.Turn = turn;
			msg.ID = pCmd->ID;
			msg.DestPosX = pCmd->PosX;
			msg.DestPosY = pCmd->PosY;
			msg.Turn = turn;
			mpClient->PostSend(false, msg.Size, (char*)&msg);
		}
		break;
		case eCommandTag::AttackUnit:
		{
			AttackUnitCommand* pCmd = static_cast<AttackUnitCommand*>(mVecCommand[i]);
			MsgUnitAttack msg;
			msg.Turn = turn;
			msg.ID = pCmd->ID;
			msg.TargetID = pCmd->TargetID;
			msg.Turn = turn;
			mpClient->PostSend(false, msg.Size, (char*)&msg);
		}
		break;
		case eCommandTag::Move:
		{
			MoveCommand* pCmd = static_cast<MoveCommand*>(mVecCommand[i]);
			MsgUnitMove msg;
			msg.Turn = turn;
			msg.ID = pCmd->ID;
			msg.DestPosX = pCmd->PosX;
			msg.DestPosY = pCmd->PosY;
			msg.Turn = turn;
			mpClient->PostSend(false, msg.Size, (char*)&msg);
		}
		break;
		case eCommandTag::Stop:
		{
			StopCommand* pCmd = static_cast<StopCommand*>(mVecCommand[i]);
			MsgUnitStop msg;
			msg.Turn = turn;
			msg.ID = pCmd->ID;
			msg.Turn = turn;
			mpClient->PostSend(false, msg.Size, (char*)&msg);
		}
		break;
		}
	}

	mMapTurn[turn] = CommandInfo();
	mMapTurn[turn].Commands = mVecCommand;
	mVecCommand.clear();
}

void NetworkManager::EraseAllCommand()
{
	for (int i = 0; i < mVecCommand.size(); ++i)
	{
		delete mVecCommand[i];
	}

	mVecCommand.clear();
}
