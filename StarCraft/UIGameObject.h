#pragma once
#include "GameObject.h"

class UIManager;
class UIGameObject : public GameObject
{
	friend UIManager;
protected:
	int mOrder = 0;
public:
	UIGameObject();
	virtual ~UIGameObject();

	inline int GetOrder() const { return mOrder; }
	inline void SetOrder(int order) { mOrder = order; }
};

