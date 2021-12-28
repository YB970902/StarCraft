#pragma once
#include "Singleton.h"

#define FOG FogManager::GetInstance()

class Fog;
class Scene;
class FogManager : public Singleton<FogManager>
{
	const int FOG_SIZE = 31;

	vector<Fog*> mVecFog;
	Scene* mpScene = nullptr;
	int mTileWidth = 0;
	int mTileHeight = 0;
public:
	void Init(Scene* pScene);
	void InitLayerSize(int width, int height);
	void Release();
};

