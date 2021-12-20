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

	for (auto it = mVecChild.begin(); it != mVecChild.end();)
	{
		it->second->release();
		delete it->second;
		it = mVecChild.erase(it);
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
		mVecChild[i].second->update();
	}
}

void GameObject::render(ID2D1DeviceContext2* pContext)
{
	if (!mpRenderer->IsRender()) return;

	mpRenderer->Render(pContext);

	for (int i = 0; i < mChildSize; ++i)
	{
		mVecChild[i].second->render(pContext);
	}
}

GameObject::GameObject()
{
	init();
}

GameObject::GameObject(RendererComponent* pRenderer)
{
	mpTransform = static_cast<TransformComponent*>(AddComponent(new TransformComponent()));
	mpRenderer = static_cast<RendererComponent*>(AddComponent(pRenderer));
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

GameObject* GameObject::AddChild(GameObject* pObject, int order)
{
	++mChildSize;
	pObject->GetTransform()->SetParent(mpTransform);
	mVecChild.push_back(make_pair(order, pObject));
	sort(mVecChild.begin(), mVecChild.end(),
		[](const pair<int, GameObject*>& lhs, const pair<int, GameObject*>& rhs)
		{
			return lhs.first < rhs.first;
		});
	return pObject;
}
