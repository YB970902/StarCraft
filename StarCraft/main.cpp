#include "stdafx.h"
#include "GameRoot.h"

#define _CRTDBG_MAP_ALLOC

#include <crtdbg.h>

POINT				g_ptMouse = { 0, 0 };
HINSTANCE			g_hInstance;
HWND				g_hWnd;
LPWSTR				g_lpszClass = (LPWSTR)TEXT("StarCraft");
GameRoot			g_gameRoot;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPWSTR _lpszCmdParam, int nCmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	MSG message;

	g_hInstance = _hInstance;

	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = _hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = g_lpszClass;
	wndClass.lpszMenuName = g_lpszClass;
	wndClass.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClass(&wndClass);

	g_hWnd = CreateWindow(g_lpszClass, g_lpszClass, WS_OVERLAPPEDWINDOW, WIN_START_POS_X, WIN_START_POS_Y, WIN_SIZE_X, WIN_SIZE_Y, NULL, NULL, _hInstance, NULL);

	ShowWindow(g_hWnd, nCmdShow);

	BOOL IsWindowMode = TRUE;
	WINDOWPLACEMENT wpc = {};
	LONG HWNDStyle = 0;
	LONG HWNDStyleEx = 0;
	
	SetWindowPlacement(g_hWnd, &wpc);
	
	HWNDStyle = GetWindowLong(g_hWnd, GWL_STYLE); if (HWNDStyleEx == 0)
		HWNDStyleEx = GetWindowLong(g_hWnd, GWL_EXSTYLE);
	
	LONG NewHWNDStyle = HWNDStyle;
	NewHWNDStyle &= ~WS_BORDER;
	NewHWNDStyle &= ~WS_DLGFRAME;
	NewHWNDStyle &= ~WS_THICKFRAME;
	
	LONG NewHWNDStyleEx = HWNDStyleEx;
	NewHWNDStyleEx &= ~WS_EX_WINDOWEDGE;
	
	SetWindowLong(g_hWnd, GWL_STYLE, NewHWNDStyle | WS_POPUP);
	SetWindowLong(g_hWnd, GWL_EXSTYLE, NewHWNDStyleEx | WS_EX_TOPMOST);
	ShowWindow(g_hWnd, SW_SHOWMAXIMIZED);

	g_gameRoot.Init();

	while (true)
	{
		if (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT) { break; }

			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			g_gameRoot.UpdateAndRender();
		}
	}

	g_gameRoot.Release();

	return message.wParam;
}

bool g_isFullScreen = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	static bool isUpdate = true;

	switch (iMessage)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(g_hWnd);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}