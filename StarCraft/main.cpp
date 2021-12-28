#include "stdafx.h"
#include "GameRoot.h"
#include "WindowManager.h"
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC

HWND		g_hWnd;
HINSTANCE	g_hInstance;
int			g_nCmdShow;
int			g_nWheel;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPWSTR _lpszCmdParam, int nCmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	g_hInstance = _hInstance;
	g_nCmdShow = nCmdShow;

	WindowManager windowMgr;
	windowMgr.Init((LPWSTR)TEXT("StarCraft"));

	windowMgr.Run();

	windowMgr.Release();

	return windowMgr.GetWindowMsg().wParam;
}