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

void UnitManager::Init(Scene* pScene)
{
	mpScene = pScene;

	for (int i = UNIT_ID_NONE + 1; i <= MAX_UNIT_COUNT; ++i)
	{
		mQueIDAllocater.push(i);
	}
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
	for (auto it = mVecRemovedUnit.begin(); it != mVecRemovedUnit.end();)
	{
		mpScene->RemoveGameObject((*it));
		it = mVecRemovedUnit.erase(it);
	}
}

void UnitManager::CreateUnit(eTeamTag teamTag, eUnitTag unitTag, Fix posX, Fix posY)
{
	if (mQueIDAllocater.empty())
	{
		// 여기서 경고를 띄워야함!
		// 유닛 생성 실패!!
		return;
	}

	UnitID ID = mQueIDAllocater.front();
	mQueIDAllocater.pop();

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

void UnitManager::RemoveUnit(UnitID ID)
{
	mMapUnit.erase(ID);
	mQueIDAllocater.push(ID);
}

void UnitManager::CommandAttackUnit(UnitID ID, UnitID targetID)
{
	Unit* pUnit = mMapUnit[ID];
	if (pUnit == nullptr || pUnit->GetTeamTag() != USER->GetTeamTag()) { return; }

	pUnit->ChaseTarget(targetID);
}

void UnitManager::CommandAttackGround(UnitID ID, const POINT& pos)
{
	Unit* pUnit = mMapUnit[ID];
	if (pUnit == nullptr || pUnit->GetTeamTag() != USER->GetTeamTag()) { return; }

	pUnit->MoveAlertly(pos);
}

void UnitManager::CommandMoveUnit(UnitID ID, const POINT& pos)
{
	Unit* pUnit = mMapUnit[ID];
	if (pUnit == nullptr || pUnit->GetTeamTag() != USER->GetTeamTag()) { return; }

	pUnit->Move(pos);
}

void UnitManager::CommandStopUnit(UnitID ID)
{
	Unit* pUnit = mMapUnit[ID];
	if (pUnit == nullptr || pUnit->GetTeamTag() != USER->GetTeamTag()) { return; }

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
	mMapUnit.erase(ID);

	mVecRemovedUnit.push_back(pUnit);
	USER->OnUnitRemoved(ID);
}
