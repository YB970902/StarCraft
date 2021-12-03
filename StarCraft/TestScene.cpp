#include "stdafx.h"
#include "TestScene.h"
#include "Unit.h"

void TestScene::Enter()
{
	pUnit = static_cast<Unit*>(AddGameObject(new Unit()));
}

void TestScene::Exit()
{
}

void TestScene::Update()
{
	if (INPUT->IsOnceKeyDown(VK_RBUTTON))
	{
		pUnit->SetPosition(INPUT->GetMousePosition().x, INPUT->GetMousePosition().y);
	}

	if (INPUT->IsStayKeyDown(VK_LBUTTON))
	{
		Fix x = (Fix)INPUT->GetMousePosition().x - pUnit->GetPosition().x;
		Fix y = (Fix)INPUT->GetMousePosition().y - pUnit->GetPosition().y;
		pUnit->SetRotation(RAD2DEG(atan2(static_cast<float>(y * (Fix)-1), static_cast<float>(x))));
	}

	if (INPUT->IsOnceKeyDown('T'))
	{
		pUnit->SetTargetPosition(INPUT->GetMousePosition());
		Fix x = (Fix)INPUT->GetMousePosition().x - pUnit->GetPosition().x;
		Fix y = (Fix)INPUT->GetMousePosition().y - pUnit->GetPosition().y;
		pUnit->SetRotation(RAD2DEG(atan2(static_cast<float>(y * (Fix)-1), static_cast<float>(x))));
	}

	if (INPUT->IsOnceKeyDown('Q')) { pUnit->ChangeUnitColor(EFFECT_COLOR_RED); }
	if (INPUT->IsOnceKeyDown('W')) { pUnit->ChangeUnitColor(EFFECT_COLOR_GREEN); }
	if (INPUT->IsOnceKeyDown('E')) { pUnit->ChangeUnitColor(EFFECT_COLOR_BLUE); }
	if (INPUT->IsOnceKeyDown('R')) { pUnit->ChangeUnitColor(EFFECT_COLOR_YELLOW); }
	if (INPUT->IsOnceKeyDown('A')) { pUnit->ChangeUnitColor(EFFECT_COLOR_EMERALD); }
	if (INPUT->IsOnceKeyDown('S')) { pUnit->ChangeUnitColor(EFFECT_COLOR_PURPLE); }
	if (INPUT->IsOnceKeyDown('D')) { pUnit->ChangeUnitColor(EFFECT_COLOR_WHITE); }
	if (INPUT->IsOnceKeyDown('F')) { pUnit->ChangeUnitColor(EFFECT_COLOR_BLACK); }
}
