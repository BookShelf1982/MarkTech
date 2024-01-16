cbuffer cbWorld : register(b0)
{
    float4 sunPos;
};

struct vs_out
{
    float4 pos : SV_POSITION;
    float3 norm : NORMAL;
    float2 tcoord : TEXCOORD;
};

float4 main(vs_out input) : SV_TARGET
{
    float3 sampleColor = input.norm;
    float3 ambientLight = float3(1.0f, 1.0f, 1.0f) * float1(1.0f);
    return float4(sampleColor * ambientLight, 1.0f);
}