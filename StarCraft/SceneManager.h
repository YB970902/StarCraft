#pragma once
#include "Singleton.h"

enum class eSceneTag
{
	TestScene,
};

class Scene;
class GameObject;
class SceneManager : public Singleton<SceneManager>
{
private:
	unordered_map<eSceneTag, Scene*> mMapScene;
	Scene* mpCurScene = nullptr;

public:
	void Init();
	void Release();
	void Update();
	
	void ChangeScene(eSceneTag tag);
	void AddScene(eSceneTag tag, Scene* pScene);

	GameObject* AddGameObject(GameObject* pGameObject);
	void RemoveGameObject(GameObject* pGameObject);
};