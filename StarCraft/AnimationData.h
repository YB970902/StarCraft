#pragma once

enum class eSingleClipDirection
{
	Down,
	Right,
	Repeat,
};

struct SingleAnimationClip
{
	SingleAnimationClip(eSingleClipDirection dir, int startX, int startY, int length, bool bIsLoop, Fix animTime, int repeat = 0)
		: Direction{ dir }, StartFrameX{ startX }, StartFrameY{ startY }, Length{ length }, IsLoop{ bIsLoop }, AnimationTime{ animTime }, Repeat{ repeat } {}

	eSingleClipDirection Direction = eSingleClipDirection::Right;
	int StartFrameX = 0;
	int StartFrameY = 0;
	int Length = 0;
	bool IsLoop = false;
	Fix AnimationTime = 0;
	int Repeat = 0;
};

enum class eMultipleClipDirection
{
	Up = -1,
	Down = 1,
};

struct MultipleAnimationClip
{
	MultipleAnimationClip(eMultipleClipDirection dir, int startRow, int length, bool bIsLoop, Fix animTime)
		: Direction{ dir }, StartRow { startRow }, Length{ length }, IsLoop{ bIsLoop }, AnimationTime{ animTime } {}
	eMultipleClipDirection Direction = eMultipleClipDirection::Down;
	int StartRow = 0;
	int Length = 0;
	bool IsLoop = false;
	Fix AnimationTime = 0;
};