struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 norm : NORMAL;
    float2 texcoord : TEXCOORD;
};

cbuffer CBufferPerObject
{
    float4x4 WVP;
};