#pragma once
#include "Singleton.h"
#include "AnimationData.h"

class UICursor;
class UIGameObject;
class Scene;
class UIManager : public Singleton<UIManager>
{
private:
	UIGameObject* mpCursor;
	vector<UIGameObject*> mVecUI;
public:
	virtual ~UIManager() = default;
	void Init();
	void Release();
	void Update();
	void Render(ID2D1DeviceContext2* pContext);

	void ChangeCursorState(eCursorState state);
	UIGameObject* CreateSprite(eBitmapTag bitmapTag, int order = 0, D2D_POINT_2F anchor = D2D1::Point2F(0.5f, 0.5f));
	UIGameObject* CreateAnimation(eBitmapTag bitmapTag, const SingleAnimationClip& clip, int order = 0, D2D_POINT_2F anchor = D2D1::Point2F(0.5f, 0.5f));

	UIGameObject* CreateLine(const Vector2& startPos, const Vector2& endPos, int order, float width = 1.0f, D2D1::ColorF color = D2D1::ColorF::Black);
	UIGameObject* CreateRect(const Vector2& pos, const Vector2& size, const Vector2& anchor, int order, float width = 1.0f, D2D1::ColorF color = D2D1::ColorF::Black);
	UIGameObject* CreateText(const wstring& text, const Vector2& pos, const Vector2& size, int order, int fontSize = 14, D2D1::ColorF color = D2D1::ColorF::Black, eTextAlign align = eTextAlign::Center);

	void RemoveUI(UIGameObject* pObject);
private:
	void AddUIObject(UIGameObject* pObject, int order = 0);
};

