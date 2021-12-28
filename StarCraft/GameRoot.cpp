#include "stdafx.h"
#include "GameRoot.h"
#include "RenderManager.h"

void GameRoot::Init()
{
	SOUND->Init();
	CAMERA->Init();
	RENDER->Init();
	SCENE->Init();

	TIME->Init(30);
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
