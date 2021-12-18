#pragma once

class GameObject;
class SceneManager;
class Scene abstract
{
private:
	friend SceneManager;

	vector<GameObject*> mVecGameObject;
	void init();
	void release();
	void update();
public:
	GameObject* AddGameObject(GameObject* pGameObject);
	void RemoveGameObject(GameObject* pGameObject);

	virtual ~Scene() = default;

	virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual void Update() = 0;
};

