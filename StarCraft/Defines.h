#pragma once

#define INPUT InputManager::GetInstance()
#define TIME TimeManager::GetInstance()
#define SCENE SceneManager::GetInstance()

#define DELTA_TIME TimeManager::GetInstance()->GetDeltaTime()

#define SAFE_RELEASE(p) if(p){ p->Release(); delete p; p = nullptr; }
#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }

typedef int64_t Bit;
typedef uint64_t UBit;