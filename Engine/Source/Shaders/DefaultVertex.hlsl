#include "Core.hlsli"

cbuffer cbPerObject
{
    float4x4 WVP;
};

struct VS_INPUT
{
    float3 pos : POS;
    float2 texcoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    //output.pos = mul(float4(input.pos, 1), WVP);
    output.pos = float4(input.pos, 1);
    output.texcoord = input.texcoord;
	return output;
}