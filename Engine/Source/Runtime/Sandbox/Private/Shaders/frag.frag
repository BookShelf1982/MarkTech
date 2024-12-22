#version 450

layout(set = 0, binding = 0) uniform texture2D image;
layout(set = 0, binding = 1) uniform sampler imageSampler;
layout(location = 0) in vec2 inTcoord;
layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(sampler2D(image, imageSampler), inTcoord);
}