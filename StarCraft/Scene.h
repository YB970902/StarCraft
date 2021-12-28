#pragma once
#include "INetworkReceiver.h"

class GameObject;
class SceneManager;
class Scene : public INetworkReceiver
{
private:
	friend SceneManager;

	vector<GameObject*> mVecGameObject;
	void init();
	void release();
	void update();

protected:
	bool mbIsCanUpdate = true;
public:
	GameObject* AddGameObject(GameObject* pGameObject);
	void RemoveGameObject(GameObject* pGameObject);

	virtual ~Scene() = default;

	virtual void Enter() abstract;
	virtual void Exit() abstract;
	virtual void Update() abstract;

	virtual void ReceiveMessage(Message* pMsg) override {};
};

