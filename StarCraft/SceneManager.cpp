#include "stdafx.h"
#include "SceneManager.h"
#include "TestScene.h"
#include "PathFindingScene.h"
#include "MapToolScene.h"
#include "LogoScene.h"
#include "MainScene.h"
#include "ChattingScene.h"
#include "MatchingScene.h"
#include "MultiGameScene.h"

void SceneManager::Init()
{
	AddScene(eSceneTag::TestScene, new TestScene());
	AddScene(eSceneTag::LogoScene, new LogoScene());
	AddScene(eSceneTag::MainScene, new MainScene());
	AddScene(eSceneTag::PathFindingScene, new PathFindingScene());
	AddScene(eSceneTag::MapToolScene, new MapToolScene());
	AddScene(eSceneTag::ChattingScene, new ChattingScene());
	AddScene(eSceneTag::MatchingScene, new MatchingScene());
	AddScene(eSceneTag::MultiGameScene, new MultiGameScene());

	ChangeScene(eSceneTag::LogoScene);
}

void SceneManager::Release()
{
	if (mpCurScene) { mpCurScene->release(); }

	for (auto it = mMapScene.begin(); it != mMapScene.end();)
	{
		SAFE_DELETE(it->second);
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

void SceneManager::ReceiveMessage(Message* pMsg)
{
	if (mpCurScene) { mpCurScene->ReceiveMessage(pMsg); }
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
