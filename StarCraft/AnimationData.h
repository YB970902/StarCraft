#pragma once

enum class eSingleClipDirection
{
	Down,
	Right,
};

struct SingleAnimationClip
{
	SingleAnimationClip(eSingleClipDirection dir, int startX, int startY, int length, bool bIsLoop, Fix animTime)
		: Direction{ dir }, StartFrameX{ startX }, StartFrameY{ startY }, Length{ length }, IsLoop{ bIsLoop }, AnimationTime{ animTime } {}
	eSingleClipDirection Direction = eSingleClipDirection::Right;
	int StartFrameX = 0;
	int StartFrameY = 0;
	int Length = 0;
	bool IsLoop = false;
	Fix AnimationTime = 0;
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