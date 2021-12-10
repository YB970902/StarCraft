#include "stdafx.h"
#include "GameRoot.h"
#include "RenderManager.h"

void GameRoot::Init()
{
	RENDER->Init();
	SCENE->Init();

	TIME->Init(60);
}

void GameRoot::Release()
{
	RENDER->Release();
	SCENE->Release();
}

void GameRoot::Run()
{
	if (TIME->IsUpdateTime())
	{
		INPUT->Update();

		if (INPUT->IsOnceKeyDown(VK_ESCAPE)) { DestroyWindow(g_hWnd); }

		Update();
		Render();
	}
}

void GameRoot::Update()
{
	SCENE->Update();
}

void GameRoot::Render()
{
	RENDER->Render();
}
