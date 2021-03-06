#pragma once

#include <iostream>
#include <fstream>
#include <atlstr.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <list>
#include <queue>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include <Windows.h>
#include <d2d1_3.h>
#include <d2d1_3helper.h>
#include <d2d1effects.h>
#include <d2d1effects_2.h>
#include <d2d1effectauthor_1.h>
#include <d2d1effecthelpers.h>
#include <d3d11_3.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <initguid.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3d10")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "dxguid.lib")
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
#include "CameraManager.h"
#include "RandomManager.h"
#include "SoundManager.h"