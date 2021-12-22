#include "stdafx.h"
#include "UIManager.h"
#include "UICursor.h"
#include "UISprite.h"
#include "UIAnimation.h"

void UIManager::Init()
{
	mpCursor = new UICursor();
}

void UIManager::Release()
{
	SAFE_DELETE(mpCursor);

	for (auto it = mListUI.begin(); it != mListUI.end();)
	{
		SAFE_DELETE((*it));
		it = mListUI.erase(it);
	}
}

void UIManager::Update()
{
	for (auto it = mListUI.begin(); it != mListUI.end(); ++it)
	{
		(*it)->update();
	}
	mpCursor->GetTransform()->SetPosition(INPUT->GetMousePosition());
	mpCursor->update();
}

void UIManager::Render(ID2D1DeviceContext2* pContext)
{
	for (auto it = mListUI.begin(); it != mListUI.end(); ++it)
	{
		(*it)->render(pContext);
	}

	if (mpCursor) { mpCursor->render(pContext); }
}

void UIManager::ChangeCursorState(eCursorState state)
{
	static_cast<UICursor*>(mpCursor)->ChangeAnimation((int)state);
}

GameObject* UIManager::CreateSprite(eBitmapTag bitmapTag, D2D_POINT_2F anchor)
{
	GameObject* pNewObject = new UISprite(bitmapTag, anchor);
	mListUI.push_back(pNewObject);
	return pNewObject;
}

GameObject* UIManager::CreateAnimation(eBitmapTag bitmapTag, const SingleAnimationClip& clip, D2D_POINT_2F anchor)
{
	GameObject* pNewObject = new UIAnimation(bitmapTag, clip, anchor);
	mListUI.push_back(pNewObject);
	return pNewObject;
}

void UIManager::RemoveUI(GameObject* pObject)
{
	auto it = find(mListUI.begin(), mListUI.end(), pObject);
	if (it == mListUI.end()) { return; }

	SAFE_DELETE((*it));
	mListUI.erase(it);
}