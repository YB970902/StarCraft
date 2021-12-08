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

#include "InputManager.h"
#include "TimeManager.h"
#include "SceneManager.h"