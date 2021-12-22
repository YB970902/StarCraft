#include "stdafx.h"
#include "LogoScene.h"
#include "RenderManager.h"
#include "UIManager.h"

void LogoScene::Enter()
{
	UI->Init();
	RENDER->InitDefaultLayer();
	SOUND->Play(eSoundTag::TitleBackground);

	mElapsedTime = 0;

	mpBackground = UI->CreateSprite(eBitmapTag::BACKGROUND_LOGO, 0, D2D1::Point2F(0, 0));
}

void LogoScene::Exit()
{
	UI->Release();
}

void LogoScene::Update()
{
	UI->Update();

	mElapsedTime += DELTA_TIME;
	if (mElapsedTime >= mDurationSceneChangeTime)
	{
		SCENE->ChangeScene(eSceneTag::MainScene);
		return;
	}
}
