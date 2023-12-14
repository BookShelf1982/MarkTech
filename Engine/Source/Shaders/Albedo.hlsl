#include "Core.hlsli"

Texture2D my_texture : register(t0);
SamplerState my_sampler : register(s0);

float4 main(VS_OUTPUT input) : SV_TARGET
{
    return my_texture.Sample(my_sampler, input.texcoord);
};