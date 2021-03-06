#define D2D_INPUT_COUNT 1
#define D2D_INPUT0_COMPLEX

#define D2D_REQUIRES_SCENE_POSITION

#include "d2d1effecthelpers.hlsli"

cbuffer constants : register(b0)
{
	float3 groupColor : packoffset(c0);
};

D2D_PS_ENTRY(main)
{
	float4 color = D2DSampleInput(0, D2DGetInputCoordinate(0));

	if (color.g == 0 && color.r == color.b && color.a == 1)
	{
		color = float4(mul(groupColor.r, color.r), mul(groupColor.g, color.r), mul(groupColor.b, color.r), 1);
	}

	return color;
}