#pragma once
#include "Singleton.h"

enum class eInputState
{
	None,
	Up,
	Down,
	Press,
};

const int MAX_MOUSE_COUNT = 3;
const int MAX_KEY_COUNT = 256;

const int MOUSE_LEFT = 0;
const int MOUSE_CENTER = 1;
const int MOUSE_RIGHT = 2;

class InputManager : public Singleton<InputManager>
{
protected:
	eInputState mKeyState[MAX_KEY_COUNT] = {};
	POINT mMousePosition = {};

public:
	void Init();
	void Release();
	void Update();

	bool IsOnceKeyDown(int key);
	bool IsOnceKeyUp(int key);
	bool IsStayKeyDown(int key);

	POINT GetMousePosition() { return mMousePosition; }
};