#pragma once

#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <list>

using namespace std;

const int WIN_START_POS_X = 200;
const int WIN_START_POS_Y = 100;
const int WIN_SIZE_X = 1200;
const int WIN_SIZE_Y = 900;

#include "InputManager.h"
#include "TimeManager.h"

#define INPUT InputManager::GetInstance()
#define TIME TimeManager::GetInstance()

#define DELTA_TIME TimeManager::GetInstance()->GetDeltaTime()
#define SAFE_RELEASE(p) if(p){ p->Release(); delete p; }

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern POINT g_ptMouse;