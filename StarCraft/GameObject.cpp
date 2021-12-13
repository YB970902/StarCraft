#include "stdafx.h"
#include "GameObject.h"
#include "Component.h"
#include "RendererComponent.h"

void GameObject::init()
{
	mpTransform = static_cast<TransformComponent*>(AddComponent(new TransformComponent()));
	mpRenderer = static_cast<RendererComponent*>(AddComponent(new RendererComponent()));
}

void GameObject::release()
{
	Release();

	GameObject* pObj = nullptr;
	for (auto it = mVecChild.begin(); it != mVecChild.end();)
	{
		pObj = (*it);
		it = mVecChild.erase(it);
		delete pObj;
	}

	Component* pCom = nullptr;
	for (auto it = mVecComponent.begin(); it != mVecComponent.end();)
	{
		pCom = (*it);
		it = mVecComponent.erase(it);
		SAFE_RELEASE(pCom);
	}
}

void GameObject::update()
{
	Update();

	for (int i = 0; i < mComponentSize; ++i)
	{
		mVecComponent[i]->Update();
	}

	for (int i = 0; i < mChildSize; ++i)
	{
		mVecChild[i]->update();
	}
}

GameObject::GameObject()
{
	init();
}

GameObject::~GameObject()
{
	release();
}

Component* GameObject::AddComponent(Component* pComponent)
{
	++mComponentSize;
	mVecComponent.push_back(pComponent);
	sort(mVecComponent.begin(), mVecComponent.end(),
		[](const Component* lhs, const Component* rhs)
		{
			return lhs->GetOrder() < rhs->GetOrder();
		});
	pComponent->Init(this);
	return pComponent;
}

Component* GameObject::GetComponent(eComponentTag tag)
{
	for (int i = 0; i < mVecComponent.size(); ++i)
	{
		if (mVecComponent[i]->GetTag() == tag)
		{
			return mVecComponent[i];
		}
	}

	return nullptr;
}