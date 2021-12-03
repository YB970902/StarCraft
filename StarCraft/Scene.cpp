#include "stdafx.h"
#include "Scene.h"
#include "GameObject.h"

void Scene::init()
{
	Enter();
}

void Scene::release()
{
	Exit();
	GameObject* pGameObject;
	for (auto it = mVecGameObject.begin(); it != mVecGameObject.end();)
	{
		pGameObject = (*it);
		it = mVecGameObject.erase(it);
		delete pGameObject;
	}
}

void Scene::update()
{
	for (int i = 0; i < mVecGameObject.size(); ++i)
	{
		mVecGameObject[i]->update();
	}
	Update();
}

GameObject* Scene::AddGameObject(GameObject* pGameObject)
{
	mVecGameObject.push_back(pGameObject);
	return pGameObject;
}

void Scene::RemoveGameObject(GameObject* pGameObject)
{
	for (auto it = mVecGameObject.begin(); it != mVecGameObject.end();++it)
	{
		if ((*it) == pGameObject)
		{
			mVecGameObject.erase(it);
			delete pGameObject;
			return;
		}
	}
}
