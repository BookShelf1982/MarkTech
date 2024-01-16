cbuffer cbWorld : register(b0)
{
    float4 sunPos;
}

cbuffer cbObject : register(b1)
{
    float4 color;
}

struct vs_out
{
    float4 pos : SV_POSITION;
    float3 norm : NORMAL;
    float2 tcoord : TEXCOORD;
};

float4 main(vs_out input) : SV_TARGET
{
    float3 sundir = sunPos.rgb;
    float3 finalColor = dot(input.norm, sundir) * float3(1.0f, 1.0f, 1.0f) * sunPos.a;
    
    return float4(finalColor, color.a);
}