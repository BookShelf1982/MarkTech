/* vertex attributes go here to input to the vertex shader */
struct vs_in
{
    float4 position_local : POS;
    float3 color : COL;
};

/* outputs from vertex shader go here. can be interpolated to pixel shader */
struct vs_out
{
    float4 position_clip : SV_POSITION; // required output of VS
    float3 color : COL;
};

vs_out vs_main(vs_in input)
{
    vs_out output = (vs_out) 0; // zero the memory first
    output.position_clip = input.position_local;
    output.color = input.color;
    return output;
}

float4 ps_main(vs_out input) : SV_TARGET
{
    return float4(input.color, 1.0f); // must return an RGBA colour

}