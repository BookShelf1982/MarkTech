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

layout(location = 0) out vec3 outPos;
layout(location = 1) out vec3 outNorm;
layout(location = 2) out vec2 outTCoord;

void main() {
	gl_Position = fUBO.proj * fUBO.view * oUBO.world * vec4(inPos, 1.0);;
	vec3 transformedNorm = normalize(oUBO.world * vec4(inNorm, 0.0)).rgb;
	outNorm = transformedNorm;
	outTCoord = inTCoord;
	
	vec4 transformedPos = fUBO.view * oUBO.world * vec4(inPos, 1.0);
	outPos = transformedPos.rgb / transformedPos.w;
}