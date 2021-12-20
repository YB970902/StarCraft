#include "stdafx.h"
#include "UIManager.h"
#include "UICursor.h"

void UIManager::Init()
{
	mpCursor = new UICursor();
}

void UIManager::Release()
{
	SAFE_DELETE(mpCursor);
}

void UIManager::Update()
{
	mpCursor->GetTransform()->SetPosition(INPUT->GetMousePosition());
	mpCursor->update();
}

void UIManager::ChangeCursorState(eCursorState state)
{
	mpCursor->ChangeAnimation((int)state);
}
