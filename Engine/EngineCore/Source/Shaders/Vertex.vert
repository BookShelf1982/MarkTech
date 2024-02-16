#version 450

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNorm;
layout(location = 2) in vec2 inTCoord;

layout(location = 0) out vec3 outPos;
layout(location = 1) out vec3 outNorm;
layout(location = 2) out vec2 outTCoord;

void main() {
	//Main
    gl_Position = vec4(inPos, 0.0, 1.0);	
    fragColor = inColor;
}