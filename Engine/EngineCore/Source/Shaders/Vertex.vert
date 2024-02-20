#version 450

layout(binding = 0) uniform TransformUBO 
{
	mat4 WVP;
	mat4 World;
	
} tUBO;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNorm;
layout(location = 2) in vec2 inTCoord;

layout(location = 0) out vec4 outNorm;
layout(location = 1) out vec2 outTCoord;

void main() {
	//Main
    gl_Position = tUBO.WVP * vec4(inPos, 1.0);
	outNorm = normalize(tUBO.World * vec4(inNorm, 0.0));
	outTCoord = inTCoord;
}