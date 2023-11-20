#include "Core.hlsli"

Texture2D my_texture;
SamplerState my_sampler;

float4 main(VS_OUTPUT input) : SV_TARGET
{
    return my_texture.Sample(my_sampler, input.texcoord);
};