#include "stdafx.h"
#include "UIManager.h"
#include "UIGameObject.h"
#include "RenderManager.h"
#include "UICursor.h"
#include "UISprite.h"
#include "UIAnimation.h"
#include "LineGizmo.h"
#include "RectGizmo.h"
#include "TextGizmo.h"

void UIManager::Init()
{
	mpCursor = new UICursor();
}

void UIManager::Release()
{
	SAFE_DELETE(mpCursor);

	for (auto it = mVecUI.begin(); it != mVecUI.end();)
	{
		SAFE_DELETE((*it));
		it = mVecUI.erase(it);
	}
}

void UIManager::Update()
{
	for (auto it = mVecUI.begin(); it != mVecUI.end(); ++it)
	{
		(*it)->update();
	}
	mpCursor->GetTransform()->SetPosition(INPUT->GetMousePosition());
	mpCursor->update();
}

void UIManager::Render(ID2D1DeviceContext2* pContext)
{
	for (auto it = mVecUI.begin(); it != mVecUI.end(); ++it)
	{
		if ((*it)->render(pContext))
		{
			(*it)->Render(pContext);
		}
	}

	if (mpCursor) { mpCursor->render(pContext); }
}

void UIManager::ChangeCursorState(eCursorState state)
{
	static_cast<UICursor*>(mpCursor)->ChangeAnimation((int)state);
}

UIGameObject* UIManager::CreateSprite(eBitmapTag bitmapTag, int order, D2D_POINT_2F anchor)
{
	UIGameObject* pNewObject = new UISprite(bitmapTag, anchor);
	AddUIObject(pNewObject, order);
	return pNewObject;
}

UIGameObject* UIManager::CreateAnimation(eBitmapTag bitmapTag, const SingleAnimationClip& clip, int order, D2D_POINT_2F anchor)
{
	UIGameObject* pNewObject = new UIAnimation(bitmapTag, clip, anchor);
	AddUIObject(pNewObject, order);
	return pNewObject;
}

UIGameObject* UIManager::CreateLine(const Vector2& startPos, const Vector2& endPos, int order, float width, D2D1::ColorF color)
{
	UIGameObject* pNewObject = new LineGizmo(startPos, endPos, width, RENDER->GetSolidBrush(width, color));
	AddUIObject(pNewObject, order);
	return pNewObject;
}

UIGameObject* UIManager::CreateRect(const Vector2& pos, const Vector2& size, const Vector2& anchor, int order, float width, D2D1::ColorF color)
{
	UIGameObject* pNewObject = new RectGizmo(pos, size, anchor, width, RENDER->GetSolidBrush(width, color));
	AddUIObject(pNewObject, order);
	return pNewObject;
}

UIGameObject* UIManager::CreateText(const Vector2& pos, const Vector2& size, int order, int width, int fontSize, D2D1::ColorF color, eTextAlign align)
{
	UIGameObject* pNewObject = new TextGizmo(RENDER->GetTextFormat(fontSize, color, align), TEXT(""), pos, size, RENDER->GetSolidBrush(width, color));
	AddUIObject(pNewObject, order);
	return pNewObject;
}

void UIManager::RemoveUI(UIGameObject* pObject)
{
	auto it = find(mVecUI.begin(), mVecUI.end(), pObject);
	if (it == mVecUI.end()) { return; }

	SAFE_DELETE((*it));
	mVecUI.erase(it);
}

void UIManager::AddUIObject(UIGameObject* pObject, int order)
{
	pObject->SetOrder(order);
	mVecUI.push_back(pObject);
	sort(mVecUI.begin(), mVecUI.end(),
		[](const UIGameObject* lhs, const UIGameObject* rhs)
		{
			return lhs->GetOrder() < rhs->GetOrder();
		}
	);
}
