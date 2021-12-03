#pragma once

enum class eBitmapTag
{
	UNIT_MARINE_L,
	UNIT_MARINE_R,
	UNIT_BATTLE_L,
	UNIT_BATTLE_R,
};

enum class eEffectTag
{
	COLOR_REPLACE,
	SHADOW,
};

enum class eComponentTag
{
	None,
	Effect,
	Transform,
	Renderer,
	Sprite,
	PairSprite,
	Animator,
};

enum class eUnitLayer
{
	Terrain,
	Ground,
	Sky,
};

const float EFFECT_COLOR_RED[] = { 1.0f, 0.0f, 0.0f };
const float EFFECT_COLOR_GREEN[] = { 0.0f, 1.0f, 0.0f };
const float EFFECT_COLOR_BLUE[] = { 0.0f, 0.0f, 1.0f };
const float EFFECT_COLOR_YELLOW[] = { 1.0f, 1.0f, 0.0f };
const float EFFECT_COLOR_EMERALD[] = { 1.0f, 0.0f, 1.0f };
const float EFFECT_COLOR_PURPLE[] = { 0.5f, 0.0f, 1.0f };
const float EFFECT_COLOR_WHITE[] = { 1.0f, 1.0f, 1.0f };
const float EFFECT_COLOR_BLACK[] = { 0.1f, 0.1f, 0.1f };