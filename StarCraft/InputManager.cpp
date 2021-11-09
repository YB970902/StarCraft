#include "stdafx.h"
#include "InputManager.h"

void InputManager::Init()
{
	Update();
}

void InputManager::Release()
{
}

void InputManager::Update()
{
	GetCursorPos(&mMousePosition);
	ScreenToClient(g_hWnd, &mMousePosition);

	for (int i = 0; i < MAX_KEY_COUNT; ++i)
	{
		if ((GetAsyncKeyState(i) & 0x8000) == 0x8000)
		{
			switch (mKeyState[i])
			{
			case eInputState::None:
			case eInputState::Up:
				mKeyState[i] = eInputState::Down;
				break;
			case eInputState::Down:
				mKeyState[i] = eInputState::Press;
				break;
			}
		}
		else
		{
			switch (mKeyState[i])
			{
			case eInputState::Up:
				mKeyState[i] = eInputState::None;
				break;
			case eInputState::Down:
			case eInputState::Press:
				mKeyState[i] = eInputState::Up;
				break;
			}
		}
	}
}

bool InputManager::IsOnceKeyDown(int key)
{
	return (mKeyState[key] == eInputState::Down);
}

bool InputManager::IsOnceKeyUp(int key)
{
	return (mKeyState[key] == eInputState::Up);
}

bool InputManager::IsStayKeyDown(int key)
{
	return (mKeyState[key] == eInputState::Down || mKeyState[key] == eInputState::Press);
}
