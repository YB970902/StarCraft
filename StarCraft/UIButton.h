#pragma once
#include "UIGameObject.h"

template<typename SceneType>
class UIButton : public UIGameObject
{
private:
	using scene_t = SceneType;
	using callback_t = void (SceneType::*)();

public:
	UIButton(scene_t* pScene, callback_t callback, eBitmapTag bitmapTag);
	virtual ~UIButton();

	virtual void Init() override;
	virtual void Release() override;
	virtual void Updaet() override;
};

template<typename SceneType>
inline UIButton<SceneType>::UIButton(scene_t* pScene, callback_t callback, eBitmapTag bitmapTag)
{
}

template<typename SceneType>
inline UIButton<SceneType>::~UIButton()
{
}

template<typename SceneType>
inline void UIButton<SceneType>::Init()
{
}

template<typename SceneType>
inline void UIButton<SceneType>::Release()
{
}

template<typename SceneType>
inline void UIButton<SceneType>::Updaet()
{
}
