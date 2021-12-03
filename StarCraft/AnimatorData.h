#pragma once
#include "AnimationData.h"

enum class eAnimationTag
{
	Idle,
	Move,
	PrepairAttack,
	PrepairIdle,
	WaitAttack,
	Attack,
};

const MultipleAnimationClip MarineIdle = MultipleAnimationClip(eMultipleClipDirection::Down, 0, 1, false, 0);
const MultipleAnimationClip MarineMove = MultipleAnimationClip(eMultipleClipDirection::Down, 4, 9, true, 0.5f);
const MultipleAnimationClip MarinePrepairAttack = MultipleAnimationClip(eMultipleClipDirection::Down, 0, 3, false, 0.2f);
const MultipleAnimationClip MarinePrepairIdle = MultipleAnimationClip(eMultipleClipDirection::Up, 2, 3, false, 0.2f);
const MultipleAnimationClip MarineWaitAttack = MultipleAnimationClip(eMultipleClipDirection::Down, 2, 1, false, 0);
const MultipleAnimationClip MarineAttack = MultipleAnimationClip(eMultipleClipDirection::Down, 3, 1, false, 0);

const MultipleAnimationClip BattlecruiserIdle = MultipleAnimationClip(eMultipleClipDirection::Down, 0, 1, false, 0);