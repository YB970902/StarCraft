#pragma once
#include "Singleton.h"
#include "AnimationData.h"

class UICursor;
class GameObject;
class Scene;
class UIManager : public Singleton<UIManager>
{
private:
	GameObject* mpCursor;
	list<GameObject*> mListUI;
public:
	virtual ~UIManager() = default;
	void Init();
	void Release();
	void Update();
	void Render(ID2D1DeviceContext2* pContext);

	void ChangeCursorState(eCursorState state);
	GameObject* CreateSprite(eBitmapTag bitmapTag, D2D_POINT_2F anchor = D2D1::Point2F(0.5f, 0.5f));
	GameObject* CreateAnimation(eBitmapTag bitmapTag, const SingleAnimationClip& clip, D2D_POINT_2F anchor = D2D1::Point2F(0.5f, 0.5f));
	void RemoveUI(GameObject* pObject);
};

