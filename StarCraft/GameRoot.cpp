#include "stdafx.h"
#include "GameRoot.h"
#include "RenderManager.h"

void GameRoot::Init()
{
	SOUND->Init();
	CAMERA->Init();
	RENDER->Init();
	SCENE->Init();

	TIME->Init(60);
}

void GameRoot::Release()
{
	RENDER->Release();
	SCENE->Release();
	SOUND->Release();
}

void GameRoot::Run()
{
	if (TIME->IsUpdateTime())
	{
		INPUT->Update();

		if (INPUT->IsOnceKeyDown(VK_F1)) { DestroyWindow(g_hWnd); }

		Update();
		CAMERA->Update();
		Render();
	}
}

void GameRoot::Update()
{
	SOUND->Update();
	SCENE->Update();
}

void GameRoot::Render()
{
	RENDER->Render();
}
