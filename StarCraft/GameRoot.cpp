#include "stdafx.h"
#include "GameRoot.h"

void GameRoot::Init()
{
}

void GameRoot::Release()
{
}

void GameRoot::UpdateAndRender()
{
	if (TIME->IsUpdateTime())
	{
		Update();
		Render();
	}
}

void GameRoot::Update()
{
}

void GameRoot::Render()
{
}
