#include "stdafx.h"
#include "LogoScene.h"
#include "RenderManager.h"

void LogoScene::Enter()
{
	RENDER->InitDefaultLayer();
	SOUND->Play(eSoundTag::TitleBackground);

	mElapsedTime = 0;

	mpBackground = UI->CreateSprite(eBitmapTag::BACKGROUND_LOGO, 0, D2D1::Point2F(0, 0));
}

void LogoScene::Exit()
{
	UI->RemoveUI(mpBackground);
}

void LogoScene::Update()
{
	mElapsedTime += DELTA_TIME;
	if (mElapsedTime >= mDurationSceneChangeTime)
	{
		SCENE->ChangeScene(eSceneTag::MainScene);
		return;
	}
}
