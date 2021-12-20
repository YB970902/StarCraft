#include "stdafx.h"
#include "UnitManager.h"
#include "Unit.h"
#include "Scene.h"
#include "UserManager.h"

void UnitManager::Init(Scene* pScene)
{
	mpScene = pScene;
}

void UnitManager::Release()
{
	mpScene = nullptr;
}

void UnitManager::Update()
{

}

void UnitManager::CreateUnit(eTeamTag teamTag, eUnitTag unitTag, Fix posX, Fix posY, UnitID ID)
{
	Unit* newUnit = nullptr;
	switch (unitTag)
	{
	case eUnitTag::Marrine:
		newUnit = new Unit(teamTag, ID);
		break;
	case eUnitTag::Goliaht:
		newUnit = new Unit(teamTag, ID);
		break;
	default:
		break;
	}
	mpScene->AddGameObject(newUnit);
	newUnit->SetPosition(posX, posY);
	newUnit->Stop();

	mMapUnit[ID] = newUnit;
}

void UnitManager::RemoveUnit(UnitID ID)
{
	mMapUnit.erase(ID);

}

void UnitManager::MoveUnit(UnitID ID, const POINT& pos)
{
	Unit* pUnit = mMapUnit[ID];
	if (pUnit == nullptr) { return; }
	
	pUnit->Move(pos);
}

void UnitManager::StopUnit(UnitID ID)
{
	Unit* pUnit = mMapUnit[ID];
	if (pUnit == nullptr) { return; }

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
	if (it == mMapUnit.end()) return false;

	position = POINT{ it->second->GetPosition().x, it->second->GetPosition().y };
}
