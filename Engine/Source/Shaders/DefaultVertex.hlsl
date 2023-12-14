#include "Core.hlsli"

struct VS_INPUT
{
    float3 pos : POS;
    float3 norm : NORMAL;
    float2 texcoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.pos = mul(float4(input.pos, 1), WVP);
    //output.pos = float4(input.pos, 1);
    output.norm = input.norm;
    output.texcoord = input.texcoord;
	return output;
}