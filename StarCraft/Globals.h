#pragma once

extern HWND			g_hWnd;
extern HINSTANCE	g_hInstance;
extern int			g_nCmdShow;
extern int			g_nWheel;

const int GIZMO_SIZE = 20;
const int BIT_COUNT = sizeof(Bit) * 8;
const Bit FULL_BIT = 9'223'372'036'854'775'807;
const Bit CLEAR_BIT = 0;
const Bit NPOS = ~(0);