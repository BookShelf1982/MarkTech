#version 450

layout(set = 0, binding = 0) uniform PerFrameBuffer {
	mat4 view;
	mat4 proj;
	vec3 viewPos;
} fUBO;

layout(set = 0, binding = 1) uniform PerObjectBuffer {
	mat4 world;
	vec3 color;
} oUBO;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNorm;
layout(location = 2) in vec2 inTCoord;

layout(location = 0) out vec3 outNorm;
layout(location = 1) out vec2 outTCoord;
layout(location = 2) out vec3 outColor;

float blinn_phong(vec3 viewDir, vec3 lightDir, vec3 surfaceNormal, float exponent) {
	float specular = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	specular = pow(max(dot(surfaceNormal, halfwayDir), 0.0), exponent);
	return specular;
}

void main() {
	gl_Position = fUBO.proj * fUBO.view * oUBO.world * vec4(inPos, 1.0);;
	vec3 transformedNorm = normalize(oUBO.world * vec4(inNorm, 0.0)).rgb;
	outNorm = transformedNorm;
	outTCoord = inTCoord;
	
	vec4 transformedPos = fUBO.view * oUBO.world * vec4(inPos, 1.0);
	vec3 vertPos = transformedPos.rgb / transformedPos.w;
	
	vec3 viewDir = normalize(fUBO.viewPos - inPos);
	vec3 lightDir = vec3(0.0, 0.0, 1.0);
	float spec = blinn_phong(viewDir, lightDir, transformedNorm, 128.0);
	float diff = max(dot(transformedNorm, lightDir), 0.0);
	outColor = spec + (diff * oUBO.color);
}