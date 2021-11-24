#pragma once
#include "Singleton.h"
#include "ObserverPattern.h"

enum class eSceneTag
{
	None,
	Login,
	Lobby,
	CreateRoom,
	Room,
};

class IScene abstract
{
public:
	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual void Notice(Message* pMsg) = 0;
};

class SceneManager : public Singleton<SceneManager>, public IObserver
{
private:
	std:: unordered_map<eSceneTag, IScene*> mScenes;
	IScene* mpCurScene = nullptr;
public:
	void Init();
	void Release();
	void Update();
	void Render();

	void AddScene(eSceneTag tag, IScene* pScene);
	void ChangeScene(eSceneTag tag);

	virtual void Notice(Message* pMsg) override;
};