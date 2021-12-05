#include "stdafx.h"
#include "TestScene.h"
#include "Unit.h"
#include "RenderManager.h"

void TestScene::Enter()
{
	pUnit = static_cast<Unit*>(AddGameObject(new Unit()));

	RENDER->RenderText(TEXT("Text Test"), Vector2(100, 100), Vector2(100, 50));
	RENDER->RenderText(TEXT("텍스트 테스트"), Vector2(100, 150), Vector2(100, 50));

	RENDER->RenderRect(Vector2(100, 100), Vector2(100, 50), D2D1::ColorF(0, 0, 0, 1), Vector2(0, 0));
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

	if (INPUT->IsStayKeyDown(VK_LEFT)) { RENDER->AddCameraPosition(Vector2::Right() * 100 * DELTA_TIME); }
	if (INPUT->IsStayKeyDown(VK_RIGHT)) { RENDER->AddCameraPosition(Vector2::Left() * 100 * DELTA_TIME); }
	if (INPUT->IsStayKeyDown(VK_UP)) { RENDER->AddCameraPosition(Vector2::Down() * 100 * DELTA_TIME); }
	if (INPUT->IsStayKeyDown(VK_DOWN)) { RENDER->AddCameraPosition(Vector2::Up() * 100 * DELTA_TIME); }

	if (INPUT->IsOnceKeyDown('Q')) { pUnit->ChangeUnitColor(EFFECT_COLOR_RED); }
	if (INPUT->IsOnceKeyDown('W')) { pUnit->ChangeUnitColor(EFFECT_COLOR_GREEN); }
	if (INPUT->IsOnceKeyDown('E')) { pUnit->ChangeUnitColor(EFFECT_COLOR_BLUE); }
	if (INPUT->IsOnceKeyDown('R')) { pUnit->ChangeUnitColor(EFFECT_COLOR_YELLOW); }
	if (INPUT->IsOnceKeyDown('A')) { pUnit->ChangeUnitColor(EFFECT_COLOR_EMERALD); }
	if (INPUT->IsOnceKeyDown('S')) { pUnit->ChangeUnitColor(EFFECT_COLOR_PURPLE); }
	if (INPUT->IsOnceKeyDown('D')) { pUnit->ChangeUnitColor(EFFECT_COLOR_WHITE); }
	if (INPUT->IsOnceKeyDown('F')) { pUnit->ChangeUnitColor(EFFECT_COLOR_BLACK); }
}
