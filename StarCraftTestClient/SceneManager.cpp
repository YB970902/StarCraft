#include "stdafx.h"
#include "SceneManager.h"

#include "LoginScene.h"
#include "LobbyScene.h"
#include "CreateRoomScene.h"
#include "RoomScene.h"

void SceneManager::Init()
{
	AddScene(eSceneTag::Login, new LoginScene);
	AddScene(eSceneTag::Lobby, new LobbyScene);
	AddScene(eSceneTag::CreateRoom, new CreateRoomScene);
	AddScene(eSceneTag::Room, new RoomScene);
}

void SceneManager::Release()
{
	for (auto it = mScenes.begin(); it != mScenes.end(); ++it)
	{
		if (it->second)
		{
			delete it->second;
			it->second = nullptr;
		}
	}
}

void SceneManager::Update()
{
	if (mpCurScene) { mpCurScene->Update(); }
}

void SceneManager::Render()
{
	if (mpCurScene) { mpCurScene->Render(); }
}

void SceneManager::AddScene(eSceneTag tag, IScene* pScene)
{
	if (mpCurScene == nullptr)
	{
		mpCurScene = pScene;
		mpCurScene->Init();
	}
	mScenes[tag] = pScene;
}

void SceneManager::ChangeScene(eSceneTag tag)
{
	mpCurScene->Release();
	mpCurScene = mScenes[tag];
	mpCurScene->Init();
}

void SceneManager::Notice(Message* pMsg)
{
	if (mpCurScene)
	{
		mpCurScene->Notice(pMsg);
	}
}
