#pragma once

#define INPUT InputManager::GetInstance()
#define TIME TimeManager::GetInstance()
#define SCENE SceneManager::GetInstance()
#define CAMERA CameraManager::GetInstance()
#define RANDOM RandomManager::GetInstance()
#define UI UIManager::GetInstance()
#define USER UserManager::GetInstance()
#define SOUND SoundManager::GetInstance()
#define NET NetworkManager::GetInstance()

#define DELTA_TIME TimeManager::GetInstance()->GetDeltaTime()

#define SAFE_RELEASE(p) if(p){ p->Release(); delete p; p = nullptr; }
#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }

typedef int64_t Bit;
typedef uint64_t UBit;
typedef unsigned int UnitID;