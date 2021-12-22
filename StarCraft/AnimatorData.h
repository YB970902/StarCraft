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

const SingleAnimationClip CursorIdle = SingleAnimationClip(eSingleClipDirection::Right, 0, 0, 5, true, 1);
const SingleAnimationClip CursorDrag = SingleAnimationClip(eSingleClipDirection::Right, 0, 1, 4, true, 1);
const SingleAnimationClip CursorOnGreen = SingleAnimationClip(eSingleClipDirection::Right, 0, 2, 14, true, 1);
const SingleAnimationClip CursorOnRed = SingleAnimationClip(eSingleClipDirection::Right, 0, 3, 14, true, 1);
const SingleAnimationClip CursorOutDown = SingleAnimationClip(eSingleClipDirection::Right, 0, 4, 2, true, 1);
const SingleAnimationClip CursorOutLeftDown = SingleAnimationClip(eSingleClipDirection::Right, 2, 4, 2, true, 1);
const SingleAnimationClip CursorOutRightDown = SingleAnimationClip(eSingleClipDirection::Right, 4, 4, 2, true, 1);
const SingleAnimationClip CursorOutLeft = SingleAnimationClip(eSingleClipDirection::Right, 6, 4, 2, true, 1);
const SingleAnimationClip CursorOutRight = SingleAnimationClip(eSingleClipDirection::Right, 0, 5, 2, true, 1);
const SingleAnimationClip CursorOutUp = SingleAnimationClip(eSingleClipDirection::Right, 2, 5, 2, true, 1);
const SingleAnimationClip CursorOutUpLeft = SingleAnimationClip(eSingleClipDirection::Right, 4, 5, 2, true, 1);
const SingleAnimationClip CursorOutUpRight = SingleAnimationClip(eSingleClipDirection::Right, 6, 5, 2, true, 1);
const SingleAnimationClip CursorPrepairGreen = SingleAnimationClip(eSingleClipDirection::Right, 0, 6, 2, true, 1);
const SingleAnimationClip CursorPrepairRed = SingleAnimationClip(eSingleClipDirection::Right, 2, 6, 2, true, 1);
const SingleAnimationClip CursorPrepairYellow = SingleAnimationClip(eSingleClipDirection::Right, 4, 6, 2, true, 1);

const SingleAnimationClip UIButtonSingle = SingleAnimationClip(eSingleClipDirection::Repeat, 0, 0, 5, true, 0.5f, 7);
const SingleAnimationClip UIButtonOnSingle = SingleAnimationClip(eSingleClipDirection::Repeat, 0, 0, 10, true, 0.5f, 6);
const SingleAnimationClip UIButtonMulti = SingleAnimationClip(eSingleClipDirection::Repeat, 0, 0, 5, true, 0.5f, 10);
const SingleAnimationClip UIButtonOnMulti = SingleAnimationClip(eSingleClipDirection::Repeat, 0, 0, 5, true, 0.5f, 4);
const SingleAnimationClip UIButtonEditor = SingleAnimationClip(eSingleClipDirection::Repeat, 0, 0, 5, true, 0.5f, 17);
const SingleAnimationClip UIButtonOnEditor = SingleAnimationClip(eSingleClipDirection::Repeat, 0, 0, 5, true, 0.5f, 4);
const SingleAnimationClip UIButtonExit = SingleAnimationClip(eSingleClipDirection::Repeat, 0, 0, 5, true, 0.5f, 10);
const SingleAnimationClip UIButtonOnExit = SingleAnimationClip(eSingleClipDirection::Repeat, 0, 0, 5, true, 0.5f, 6);

const SingleAnimationClip ClickCircle = SingleAnimationClip(eSingleClipDirection::Right, 0, 0, 5, false, 0.2f);

const SingleAnimationClip ParticleAttack = SingleAnimationClip(eSingleClipDirection::Right, 0, 0, 15, false, 1);
const SingleAnimationClip ParticleExplosion = SingleAnimationClip(eSingleClipDirection::Right, 0, 0, 9, false, 1);
const SingleAnimationClip ParticleMarineDead = SingleAnimationClip(eSingleClipDirection::Right, 0, 0, 7, false, 0.5f);

const SingleAnimationClip RemainsMarine = SingleAnimationClip(eSingleClipDirection::Right, 0, 0, 3, false, 5);

const MultipleAnimationClip MarineIdle = MultipleAnimationClip(eMultipleClipDirection::Down, 0, 1, false, 0);
const MultipleAnimationClip MarineMove = MultipleAnimationClip(eMultipleClipDirection::Down, 4, 9, true, 0.5f);
const MultipleAnimationClip MarinePrepairAttack = MultipleAnimationClip(eMultipleClipDirection::Down, 0, 3, false, 0.2f);
const MultipleAnimationClip MarinePrepairIdle = MultipleAnimationClip(eMultipleClipDirection::Up, 2, 3, false, 0.2f);
const MultipleAnimationClip MarineWaitAttack = MultipleAnimationClip(eMultipleClipDirection::Down, 2, 1, false, 0);
const MultipleAnimationClip MarineAttack = MultipleAnimationClip(eMultipleClipDirection::Down, 3, 1, false, 0);