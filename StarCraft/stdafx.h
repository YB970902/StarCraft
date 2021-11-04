#pragma once

#include <SDKDDKVer.h>
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <d2d1.h>
#include <d3d9.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <list>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "D2D1.lib")
#pragma comment(lib, "dwrite.lib")

using namespace D2D1;
using namespace std;

const int WIN_START_POS_X = 200;
const int WIN_START_POS_Y = 100;
const int WIN_SIZE_X = 1200;
const int WIN_SIZE_Y = 900;

#include "InputManager.h"
#include "TimeManager.h"

#define INPUT InputManager::GetInstance()
#define TIME TimeManager::GetInstance()

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern POINT g_ptMouse;