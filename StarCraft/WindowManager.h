#pragma once

class GameRoot;
class WindowManager
{
private:
	GameRoot* mpGameRoot = nullptr;
	MSG mMessage;
	WNDCLASS mWndClass;
	LPWSTR mpTitle;
public:
	void Init(LPWSTR title);
	void Release();

	void Run();

	inline MSG GetWindowMsg() { return mMessage; }

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
private:
	void CreateWindowClass();
	void RenderWindow();

};