#version 450

layout(location = 0) in vec3 inNorm;
layout(location = 1) in vec2 inTCoord;
layout(location = 2) in vec3 inColor;

layout(set = 1, binding =  3) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(inColor, 1.0) * texture(texSampler, inTCoord);
}