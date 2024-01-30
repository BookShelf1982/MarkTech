/*cbuffer cbObject : register(b0)
{
    row_major float4x4 World;
    row_major float4x4 WVP;
};*/

struct vs_in
{
    float3 pos : POSITION;
    float3 norm : NORMAL;
    float2 tcoord : TEXCOORD;
};

struct vs_out
{
    float4 pos : SV_POSITION;
    float3 norm : NORMAL;
    float2 tcoord : TEXCOORD;
};

vs_out main(vs_in input)
{
    vs_out output = (vs_out)0;
    //output.pos = mul(float4(input.pos, 1.0), WVP);
    //output.norm = normalize(mul(float4(input.norm, 0.0f), World)).rgb;
    output.pos = float4(input.pos, 1.0f);
    output.norm = input.norm;
    output.tcoord = input.tcoord;
    return output;
}