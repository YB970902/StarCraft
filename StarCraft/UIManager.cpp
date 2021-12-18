#include "stdafx.h"
#include "UIManager.h"
#include "UICursor.h"

void UIManager::Init()
{
	mpCursor = new UICursor();
	mpCursor->GetRenderer()->SetUnitLayer(eUnitLayer::Sky);
}

void UIManager::Release()
{
	SAFE_DELETE(mpCursor);
}

void UIManager::Update()
{
	mpCursor->update();
	mpCursor->GetTransform()->SetPosition(INPUT->GetLocalMousePosition());
}

void UIManager::ChangeCursorState(eCursorState state)
{
	mpCursor->ChangeAnimation((int)state);
}
