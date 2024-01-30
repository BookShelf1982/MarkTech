struct vs_out
{
    float4 pos : SV_POSITION;
    float3 norm : NORMAL;
    float2 tcoord : TEXCOORD;
};

float4 main(vs_out input) : SV_TARGET
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}