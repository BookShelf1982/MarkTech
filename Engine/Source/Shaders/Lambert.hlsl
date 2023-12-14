#include "Core.hlsli"

Texture2D basetexture : register(t0);
SamplerState texturesampler : register(s0);

float4 main(VS_OUTPUT input) : SV_TARGET
{
    float4 basecolor = basetexture.Sample(texturesampler, input.texcoord);
    float3 sundir = float3(-1.0f, 1.0f, -1.0f);
    float1 DotProductThing = dot(input.norm, sundir);
    float4 finalColor = DotProductThing * basecolor * 2.0f;
    
    return finalColor;
}