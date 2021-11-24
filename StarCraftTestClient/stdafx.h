#pragma once
#include <iostream>
#include <deque>
#include <vector>
#include <queue>
#include <conio.h>
#include <unordered_map>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "Fixed.h"
typedef numeric::fixed<16, 16> fix;

#include "../StarCraftServer/Protocol.h"
#include "TimeManager.h"
#include "NetworkManager.h"
#include "SceneManager.h"
#include "UserManager.h"

#define TIME TimeManager::GetInstance()
#define DELTA_TIME TIME->GetDeltaTime()
#define NET NetworkManager::GetInstance()
#define SCENE SceneManager::GetInstance()
#define USER UserManager::GetInstance()

#define SAFE_DELETE(p) (if(p) { delete p; p = nullptr; })
#define SAFE_RELEASE(p) (if(p) { p->Release(); delete p; p = nullptr; })