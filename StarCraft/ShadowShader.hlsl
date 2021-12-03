#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_COMPLEX

#define D2D_REQUIRES_SCENE_POSITION

#include "d2d1effecthelpers.hlsli"

D2D_PS_ENTRY(main)
{
	float4 color = D2DSampleInput(0, D2DGetInputCoordinate(0));

	if (color.a == 1)
	{
		color = float4(0, 0, 0, 0.5f);
	}

	return color;
}