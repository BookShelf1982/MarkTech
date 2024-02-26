#version 450

layout(set = 0, binding = 0) uniform PerFrameBuffer {
	mat4 view;
	mat4 proj;
	vec3 viewPos;
} fUBO;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNorm;
layout(location = 2) in vec2 inTCoord;

layout(set = 1, binding =  3) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;

float blinn_phong(vec3 viewDir, vec3 lightDir, float exponent) {
	float specular = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	specular = pow(max(dot(inNorm, halfwayDir), 0.0), exponent);
	return specular;
}

void main() {
	vec3 lightDir = vec3(0.75, 0.8, 1.0);
	vec3 viewDir = normalize(fUBO.viewPos - inPos);
	
	float spec = blinn_phong(viewDir, lightDir, 128.0);
	
	float diff = max(dot(inNorm, lightDir), 0.0);
	
	vec3 diffuse = vec3(1.0, 1.0, 1.0) * diff;
	
	vec3 result = (spec * 8.0) + (vec3(0.025, 0.025, 0.025) + diffuse);
	
    outColor = vec4(result, 1.0) * texture(texSampler, inTCoord * 0.5);
}