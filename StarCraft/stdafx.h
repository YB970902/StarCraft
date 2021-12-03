#pragma once

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
#include <queue>

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

#include "Math.h"

using namespace std;

const int WIN_START_POS_X = 200;
const int WIN_START_POS_Y = 100;
const int WIN_SIZE_X = 1200;
const int WIN_SIZE_Y = 900;

#include "InputManager.h"
#include "TimeManager.h"
#include "SceneManager.h"

#define INPUT InputManager::GetInstance()
#define TIME TimeManager::GetInstance()
#define SCENE SceneManager::GetInstance()

#define DELTA_TIME TimeManager::GetInstance()->GetDeltaTime()
#define SAFE_RELEASE(p) if(p){ p->Release(); delete p; }

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern POINT g_ptMouse;

#include "Enums.h"