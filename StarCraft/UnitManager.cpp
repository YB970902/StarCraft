#include "stdafx.h"
#include "UnitManager.h"
#include "Unit.h"
#include "Scene.h"
#include "UserManager.h"
#include "PhysicsManager.h"
#include "TileManager.h"
#include "IObserver.h"
#include "Subject.h"
#include "Marine.h"
#include "Goliath.h"
#include "NetworkManager.h"

void UnitManager::Init(Scene* pScene)
{
	mpScene = pScene;

	for (int i = UNIT_ID_NONE + 1; i <= MAX_UNIT_COUNT; ++i)
	{
		mQueRedTeamIDAllocater.push(i);
		mQueBlueTeamIDAllocater.push(i + MAX_UNIT_COUNT);
	}

	mVecRemovedUnit.reserve(MAX_UNIT_COUNT * 2);
}

void UnitManager::Release()
{
	for (auto it = mMapUnit.begin(); it != mMapUnit.end();)
	{
		mpScene->RemoveGameObject((it->second));
		it = mMapUnit.erase(it);
	}
	mpScene = nullptr;
}

void UnitManager::Update()
{
	if (mVecRemovedUnit.empty() == false)
	{
		for (auto it = mVecRemovedUnit.begin(); it != mVecRemovedUnit.end();)
		{
			mpScene->RemoveGameObject((*it));
			it = mVecRemovedUnit.erase(it);
		}
	}
}

void UnitManager::CommandCreateUnit(eTeamTag teamTag, eUnitTag unitTag, int posX, int posY)
{
	if (teamTag == eTeamTag::RED_TEAM && mQueRedTeamIDAllocater.empty())
	{
		// 여기서 경고를 띄워야함!
		// 유닛 생성 실패!!
		return;
	}
	else if (teamTag == eTeamTag::BLUE_TEAM && mQueBlueTeamIDAllocater.empty())
	{
		// 여기서 경고를 띄워야함!
		// 유닛 생성 실패!!
		return;
	}
	NET->AddCommand(new CreateCommand(teamTag, unitTag, posX, posY));
}

void UnitManager::CommandAttackUnit(UnitID ID, UnitID targetID)
{
	Unit* pUnit = mMapUnit[ID];
	if (pUnit == nullptr || pUnit->GetTeamTag() != USER->GetTeamTag()) { return; }

	NET->AddCommand(new AttackUnitCommand(ID, targetID));
}

void UnitManager::CommandAttackGround(UnitID ID, const POINT& pos)
{
	Unit* pUnit = mMapUnit[ID];
	if (pUnit == nullptr || pUnit->GetTeamTag() != USER->GetTeamTag()) { return; }

	NET->AddCommand(new AttackGroundCommand(ID, pos));
}

void UnitManager::CommandMoveUnit(UnitID ID, const POINT& pos)
{
	Unit* pUnit = mMapUnit[ID];
	if (pUnit == nullptr || pUnit->GetTeamTag() != USER->GetTeamTag()) { return; }

	NET->AddCommand(new MoveCommand(ID, pos));
}

void UnitManager::CommandStopUnit(UnitID ID)
{
	Unit* pUnit = mMapUnit[ID];
	if (pUnit == nullptr || pUnit->GetTeamTag() != USER->GetTeamTag()) { return; }

	NET->AddCommand(new StopCommand(ID));
}

void UnitManager::ExecuteCreateUnit(eTeamTag teamTag, eUnitTag unitTag, int posX, int posY)
{
	UnitID ID = UNIT_ID_NONE;
	switch (teamTag)
	{
	case eTeamTag::RED_TEAM:
		ID = mQueRedTeamIDAllocater.front();
		mQueRedTeamIDAllocater.pop();
		break;
	case eTeamTag::BLUE_TEAM:
		ID = mQueBlueTeamIDAllocater.front();
		mQueBlueTeamIDAllocater.pop();
		break;
	}

	Unit* newUnit = nullptr;

	const float* arrColor = nullptr;

	switch (teamTag)
	{
	case eTeamTag::RED_TEAM:
		arrColor = EFFECT_COLOR_RED;
		break;
	case eTeamTag::BLUE_TEAM:
		arrColor = EFFECT_COLOR_BLUE;
		break;
	}

	switch (unitTag)
	{
	case eUnitTag::Marine:
		newUnit = new Marine(teamTag, ID, arrColor);
		break;
	case eUnitTag::Goliath:
		newUnit = new Goliath(teamTag, ID, arrColor);
		break;
	default:
		break;
	}

	Vector2 targetPos;
	if (TILE->GetSpawnPosition(Vector2(posX, posY), newUnit->GetUnitTileSize(), targetPos))
	{
		mpScene->AddGameObject(newUnit);
		newUnit->SetPosition(targetPos.x, targetPos.y);
		newUnit->Stop();

		mMapUnit[ID] = newUnit;
	}
	else
	{
		delete newUnit;
	}
}

void UnitManager::ExecuteAttackUnit(UnitID ID, UnitID targetID)
{
	mMapUnit[ID]->ChaseTarget(targetID);
}

void UnitManager::ExecuteAttackGround(UnitID ID, int posX, int posY)
{
	mMapUnit[ID]->MoveAlertly(POINT{ posX, posY });
}

void UnitManager::ExecuteMoveUnit(UnitID ID, int posX, int posY)
{
	mMapUnit[ID]->Move(POINT{ posX, posY });
}

void UnitManager::ExecuteStopUnit(UnitID ID)
{
	Unit* pUnit = mMapUnit[ID];
	pUnit->Stop();
}

void UnitManager::SetSelectUnit(UnitID ID, bool set)
{
	auto it = mMapUnit.find(ID);
	if (it != mMapUnit.end())
	{
		it->second->SetIsSelected(set);
	}
}

bool UnitManager::GetUnitPosition(UnitID ID, POINT& position)
{
	auto it = mMapUnit.find(ID);
	if (it == mMapUnit.end()) { return false; }

	position = POINT{ it->second->GetPosition().x, it->second->GetPosition().y };
	return true;
}

eUnitTag UnitManager::GetUnitTag(UnitID ID)
{
	return mMapUnit[ID]->GetUnitTag();
}

bool UnitManager::SetTargetID(UnitID ID, UnitID targetID)
{
	auto it = mMapUnit.find(targetID);
	if (it != mMapUnit.end())
	{
		mMapUnit[ID]->SetTargetID(targetID);
		return true;
	}
	return false;
}

IObserver* UnitManager::GetObserver(UnitID ID)
{
	auto it = mMapUnit.find(ID);
	if (it == mMapUnit.end()) { return nullptr; }

	return static_cast<IObserver*>(it->second);
}

bool UnitManager::BeginChase(UnitID ID, UnitID TargetID)
{
	auto it = mMapUnit.find(TargetID);
	if (it == mMapUnit.end())
	{
		return false;
	}

	it->second->OnNotify(ID, eObserverMessage::BeginChasing);
	return true;
}

bool UnitManager::EndChase(UnitID ID, UnitID TargetID)
{
	auto it = mMapUnit.find(TargetID);
	if (it == mMapUnit.end())
	{
		return false;
	}

	it->second->OnNotify(ID, eObserverMessage::EndChasing);
	return true;
}

bool UnitManager::Attack(UnitID ID, UnitID TargetID)
{
	auto target = mMapUnit.find(TargetID);

	if (target == mMapUnit.end()) { return false; }

	return target->second->OnDamaged(mMapUnit[ID]->GetAttack());
}

void UnitManager::Dead(UnitID ID)
{
	Unit* pUnit = mMapUnit[ID];
	mVecRemovedUnit.push_back(pUnit);
	mMapUnit.erase(ID);

	USER->OnUnitRemoved(ID);
	switch (pUnit->GetTeamTag())
	{
	case eTeamTag::RED_TEAM:
		mQueRedTeamIDAllocater.push(ID);
		break;
	case eTeamTag::BLUE_TEAM:
		mQueBlueTeamIDAllocater.push(ID);
		break;
	}
}
