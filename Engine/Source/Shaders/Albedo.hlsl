#include "Core.hlsli"

float4 main(VS_OUTPUT input) : SV_TARGET
{
	return float4(input.texcoord, 0.0f, 1.0f);
}