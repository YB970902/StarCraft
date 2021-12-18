#include "stdafx.h"
#include "SceneManager.h"
#include "TestScene.h"
#include "PathFindingScene.h"
#include "MapToolScene.h"

void SceneManager::Init()
{
	AddScene(eSceneTag::TestScene, new TestScene());
	AddScene(eSceneTag::PathFindingScene, new PathFindingScene());
	AddScene(eSceneTag::MapToolScene, new MapToolScene());

	ChangeScene(eSceneTag::PathFindingScene);
}

void SceneManager::Release()
{
	for (auto it = mMapScene.begin(); it != mMapScene.end();)
	{
		it->second->release();
		delete it->second;
		it = mMapScene.erase(it);
	}
}

void SceneManager::Update()
{
	if (mpCurScene) { mpCurScene->update(); }
}

void SceneManager::ChangeScene(eSceneTag tag)
{
	if (mpCurScene) { mpCurScene->release(); };
	mpCurScene = mMapScene[tag];
	mpCurScene->init();
}

void SceneManager::AddScene(eSceneTag tag, Scene* pScene)
{
	mMapScene[tag] = pScene;
}

GameObject* SceneManager::AddGameObject(GameObject* pGameObject)
{
	if (mpCurScene) return mpCurScene->AddGameObject(pGameObject);
	return nullptr;
}

void SceneManager::RemoveGameObject(GameObject* pGameObject)
{
	if (mpCurScene) mpCurScene->RemoveGameObject(pGameObject);
}
