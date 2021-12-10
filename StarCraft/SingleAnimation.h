#pragma once
#include "AnimationBase.h"
#include "AnimationData.h"

class SingleAnimation : public AnimationBase
{
public:
	virtual void Play() override;
	virtual void Stop() override;
	virtual void Update() override;
};