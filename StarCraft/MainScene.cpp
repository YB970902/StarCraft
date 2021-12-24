#include "stdafx.h"
#include "MainScene.h"
#include "RenderManager.h"
#include "UIManager.h"
#include "SingleAnimation.h"
#include "AnimatorData.h"
#include "PhysicsManager.h"
#include "UIGameObject.h"
#include "GameObject.h"

void MainScene::Enter()
{
	SOUND->Play(eSoundTag::TitleBackground);

	UI->Init();
	RENDER->InitDefaultLayer();
	PHYSICS->InitDefaultLayer();

	mpBackground = UI->CreateSprite(eBitmapTag::BACKGROUND_MAIN, 0, D2D1::Point2F(0, 0));
	mpBtnSingle = UI->CreateAnimation(eBitmapTag::UI_BTN_SINGLE, UIButtonSingle, 0, D2D1::Point2F(0, 0));
	mpBtnOnSingle = UI->CreateAnimation(eBitmapTag::UI_BTN_ON_SINGLE, UIButtonOnSingle, 0, D2D1::Point2F(0, 0));
	mpBtnSingle->GetTransform()->SetPosition(Vector2(75, 100));
	mpBtnOnSingle->GetTransform()->SetPosition(Vector2(201, 238));
	mpBtnOnSingle->GetRenderer()->SetIsRender(false);

	mpBtnMulti = UI->CreateAnimation(eBitmapTag::UI_BTN_MULTI, UIButtonMulti, 0, D2D1::Point2F(0, 0));
	mpBtnOnMulti = UI->CreateAnimation(eBitmapTag::UI_BTN_ON_MULTI, UIButtonOnMulti, 0, D2D1::Point2F(0, 0));
	mpMultiTail = UI->CreateSprite(eBitmapTag::UI_BTN_MULTI_TAIL, 0, D2D1::Point2F(0, 0));
	mpBtnMulti->GetTransform()->SetPosition(Vector2(200, 450));
	mpBtnOnMulti->GetTransform()->SetPosition(Vector2(257, 475));
	mpMultiTail->GetTransform()->SetPosition(Vector2(638, 801));
	mpBtnOnMulti->GetRenderer()->SetIsRender(false);

	mpBtnEditor = UI->CreateAnimation(eBitmapTag::UI_BTN_EDITOR, UIButtonEditor, 0, D2D1::Point2F(0, 0));
	mpBtnOnEditor = UI->CreateAnimation(eBitmapTag::UI_BTN_ON_EDITOR, UIButtonOnEditor, 0, D2D1::Point2F(0, 0));
	mpBtnEditor->GetTransform()->SetPosition(Vector2(1100, 250));
	mpBtnOnEditor->GetTransform()->SetPosition(Vector2(1163, 288));
	mpBtnOnEditor->GetRenderer()->SetIsRender(false);

	mpBtnExit = UI->CreateAnimation(eBitmapTag::UI_BTN_EXIT, UIButtonExit, 0, D2D1::Point2F(0, 0));
	mpBtnOnExit = UI->CreateAnimation(eBitmapTag::UI_BTN_ON_EXIT, UIButtonOnExit, 0, D2D1::Point2F(0, 0));
	mpBtnExit->GetTransform()->SetPosition(Vector2(1250, 750));
	mpBtnOnExit->GetTransform()->SetPosition(Vector2(1300, 744));
	mpBtnOnExit->GetRenderer()->SetIsRender(false);
}

void MainScene::Exit()
{
	PHYSICS->Release();
	UI->Release();
}

void MainScene::Update()
{
	if (INPUT->IsOnceKeyDown('1'))
	{
		SCENE->ChangeScene(eSceneTag::PathFindingScene);
		return;
	}
	if (INPUT->IsOnceKeyDown('2'))
	{
		SCENE->ChangeScene(eSceneTag::MapToolScene);
		return;
	}
	if (INPUT->IsOnceKeyDown('3'))
	{
		SCENE->ChangeScene(eSceneTag::ChattingScene);
		return;
	}
	if (INPUT->IsOnceKeyDown(VK_ESCAPE))
	{
		DestroyWindow(g_hWnd);
		return;
	}

	UI->Update();
}
