#include "stdafx.h"
#include "MainGame.h"

void MainGame::Init()
{
	SCENE->Init();
}

void MainGame::Release()
{
	SCENE->Release();
}

void MainGame::Update()
{
	NET->Update();
	SCENE->Update();
}

void MainGame::Render()
{
	SCENE->Render();
}
