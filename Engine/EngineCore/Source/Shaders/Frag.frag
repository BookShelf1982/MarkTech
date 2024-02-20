#version 450

layout(location = 0) in vec3 inNorm;
layout(location = 1) in vec2 inTCoord;

layout(location = 0) out vec4 outColor;

void main() {
	float reflectance = dot(inNorm, vec3(0.0, 0.0, 1.0));
    outColor = reflectance * vec4(1.0, 0.0, 0.0, 1.0);
}