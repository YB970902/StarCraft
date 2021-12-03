#pragma once

enum class eSingleClipDirection
{
	Up,
	Down,
	Left,
	Right,
};

struct SingleAnimationClip
{
	eSingleClipDirection Direction = eSingleClipDirection::Left;
	int StartFraemX = 0;
	int StartFrameY = 0;
	int Length = 0;
	bool IsLoop = false;
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