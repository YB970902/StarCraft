#include "stdafx.h"
#include "GameRoot.h"
#include "GameObject.h"

void GameRoot::Init()
{
	TIME->Init(60);

	mPlayer1 = new GameObject();
	mPlayer1->Init();
	mPlayer1->SetPosition({ 100, 100 });
	mPlayer2 = new GameObject();
	mPlayer2->Init();
	mPlayer2->SetPosition({ 300, 100 });
}

void GameRoot::Release()
{
	SAFE_RELEASE(mPlayer1);
	SAFE_RELEASE(mPlayer2);
}

void GameRoot::UpdateAndRender()
{
	if (TIME->IsUpdateTime())
	{
		INPUT->Update();

		Update();
		Render();
	}
}

void GameRoot::Update()
{
	if (INPUT->IsStayKeyDown(VK_LBUTTON))
	{
		mPlayer1->MoveTo(POINTFLOAT{ (float)INPUT->GetMousePosition().x, (float)INPUT->GetMousePosition().y });
	}
	mPlayer1->Update();
	mPlayer2->Update();
}

void GameRoot::Render()
{
	HDC hdc;
	hdc = GetDC(g_hWnd);


	mPlayer1->Render(hdc);
	mPlayer2->Render(hdc);

	ReleaseDC(g_hWnd, hdc);
}
