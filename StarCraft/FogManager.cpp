#include "stdafx.h"
#include "FogManager.h"
#include "RenderManager.h"
#include "Scene.h"
#include "Fog.h"

void FogManager::Init(Scene* pScene)
{
	mpScene = pScene;
}

void FogManager::InitLayerSize(int width, int height)
{
	mTileWidth = width / FOG_SIZE;
	mTileHeight = height / FOG_SIZE;

	Fog* pObject = nullptr;
	for (int x = 0; x < mTileWidth; ++x)
	{
		for (int y = 0; y < mTileHeight; ++y)
		{
			pObject = static_cast<Fog*>(mpScene->AddGameObject(new Fog));
			pObject->GetTransform()->SetPosition(x * FOG_SIZE, y * FOG_SIZE);
			mVecFog.push_back(pObject);
		}
	}

	for (int x = 0; x < 5; ++x)
	{
		for (int y = 0; y < 5; ++y)
		{
			mVecFog[x + y * mTileWidth]->SetFrameX(x);
		}
	}
}

void FogManager::Release()
{
}
