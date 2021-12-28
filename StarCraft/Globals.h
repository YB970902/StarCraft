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

const float EFFECT_COLOR_RED[] = { 1.0f, 0.0f, 0.0f };
const float EFFECT_COLOR_GREEN[] = { 0.0f, 1.0f, 0.0f };
const float EFFECT_COLOR_BLUE[] = { 0.0f, 0.0f, 1.0f };
const float EFFECT_COLOR_YELLOW[] = { 1.0f, 1.0f, 0.0f };
const float EFFECT_COLOR_EMERALD[] = { 1.0f, 0.0f, 1.0f };
const float EFFECT_COLOR_PURPLE[] = { 0.5f, 0.0f, 1.0f };
const float EFFECT_COLOR_WHITE[] = { 1.0f, 1.0f, 1.0f };
const float EFFECT_COLOR_BLACK[] = { 0.1f, 0.1f, 0.1f };

const UnitID UNIT_ID_NONE = 0;