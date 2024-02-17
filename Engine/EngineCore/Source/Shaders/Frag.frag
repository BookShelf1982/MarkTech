#version 450

layout(location = 0) in vec4 inPos;
layout(location = 1) in vec3 inNorm;
layout(location = 2) in vec2 inTCoord;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(inTCoord, 0.0, 1.0);
}