struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

cbuffer CBufferPerObject
{
    float4x4 WVP;
};