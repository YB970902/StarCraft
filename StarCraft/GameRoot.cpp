#include "stdafx.h"
#include "GameRoot.h"
#include "RenderManager.h"

void GameRoot::Init()
{
	SOUND->Init();
	CAMERA->Init();
	RENDER->Init();
	UI->Init();
	SCENE->Init();

	TIME->Init(30);
}

void GameRoot::Release()
{
	UI->Release();
	RENDER->Release();
	SCENE->Release();
	SOUND->Release();
}

void GameRoot::Run()
{
	if (TIME->IsUpdateTime())
	{
		Update();
		Render();
	}
}

void GameRoot::Update()
{
	INPUT->Update();
	SOUND->Update();
	SCENE->Update();
	UI->Update();
	CAMERA->Update();
}

void GameRoot::Render()
{
	RENDER->Render();
}
