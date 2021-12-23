#pragma once

enum class eTeamTag
{
	RED_TEAM,
	BLUE_TEAM,
};

enum class eBitmapTag
{
	NONE,

	UNIT_MARINE_L,
	UNIT_MARINE_R,
	UNIT_GOLIATH_HEAD_L,
	UNIT_GOLIATH_HEAD_R,
	UNIT_GOLIATH_LEG_L,
	UNIT_GOLIATH_LEG_R,

	BUILDING_BARRACK,
	BUILDING_FACTORY,
	BUILDING_START,

	TILE_PALETTE,

	ICON_BARRACK,
	ICON_FACTORY,

	UI_CURSOR,
	UI_CLICK_CIRCLE,
	UI_BTN_SINGLE,
	UI_BTN_ON_SINGLE,
	UI_BTN_MULTI_TAIL,
	UI_BTN_MULTI,
	UI_BTN_ON_MULTI,
	UI_BTN_EDITOR,
	UI_BTN_ON_EDITOR,
	UI_BTN_EXIT,
	UI_BTN_ON_EXIT,

	BACKGROUND_LOGO,
	BACKGROUND_MAIN,

	SELECTED_CIRCLE_SMALL,
	SELECTED_CIRCLE_MIDIUM,
	SELECTED_CIRCLE_BIG,

	PARTICLE_ATTACK,
	PARTICLE_EXPLOSION,
	PARTICLE_MARINE_DEAD,

	REMAINS_MARINE,
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
	PathFind,
	Collider,
	StateMachine,
};

enum class eUnitLayer
{
	Terrain,
	Remains,
	Ground,
	Particle,
};

enum class eUnitTileSize
{
	Small = 2,
	Big = 4,
};

enum class eUnitTag
{
	None,
	Marine,
	Goliath,
};

enum class eBuildingTag
{
	None,
	Barrack,
	Factory,
	Group1StartLocation,
	Group2StartLocation,
};

enum class eCursorState
{
	Idle,
	Drag,
	OnGreen,
	OnRed,
	OutDown,
	OutLeftDown,
	OutRightDown,
	OutLeft,
	OutRight,
	OutUp,
	OutUpLeft,
	OutUpRight,
	PrepairGreen,
	PrepairRed,
	PrepairYellow,
};

enum class eObserverMessage
{
	 BeginChasing,
	 EndChasing,
	 Dead,
};

enum class eStateTag
{
	None,
	Idle,
	Move,
	Chase,
	MoveAlertly,
	Attack,
};

enum class eParticleTag
{
	None,

	ClickCircle,

	ParticleMarineDead,
	ParticleAttack,
	ParticleExplosion,

	RemainsMarine,
};

enum class eSoundTag
{
	None,
	TerranTheme,
	TitleBackground,

	MarineDead,
	MarineAttack,
	MarineMove,
	MarineClick,

	GoliathDead,
	GoliathAttack,
	GoliathMove,
	GoliathClick,
};

enum class eTextAlign
{
	Left = 0,
	Right = 1,
	Center = 2,
};