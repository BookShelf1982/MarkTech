#version 450

layout(set = 0, binding = 0) uniform texture2D image;
layout(set = 0, binding = 1) uniform sampler imageSampler;
layout(location = 0) in vec2 inTcoord;
layout(location = 1) in flat vec3 inVert;
layout(location = 2) in mat3 inMatrix;
layout(location = 0) out vec4 outColor;

vec4 fromLinear(vec4 linearRGB)
{
    bvec3 cutoff = lessThan(linearRGB.rgb, vec3(0.0031308));
    vec3 higher = vec3(1.055)*pow(linearRGB.rgb, vec3(1.0/2.4)) - vec3(0.055);
    vec3 lower = linearRGB.rgb * vec3(12.92);

    return vec4(mix(higher, lower, cutoff), linearRGB.a);
}


void main() {
    outColor = fromLinear(texture(sampler2D(image, imageSampler), inTcoord));
	//outColor = fromLinear(vec4(inTcoord, 0.0, 1.0));
}