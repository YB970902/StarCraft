#pragma once

#include <Windows.h>
#include <d2d1_3.h>
#include <d2d1_3helper.h>
#include <d2d1effects_2.h>
#include <d2d1effectauthor_1.h>
#include <d2d1effecthelpers.h>
#include <d3d11_3.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <initguid.h>

#include <iostream>
#include <fstream>
#include <atlstr.h>
#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <list>

#include <Windows.Applicationmodel.Activation.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3d10")
#pragma comment(lib, "dwrite")
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

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