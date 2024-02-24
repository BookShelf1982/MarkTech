#version 450

layout(location = 0) in vec3 inNorm;
layout(location = 1) in vec2 inTCoord;
layout(binding = 0)

layout(location = 0) out vec4 outColor;

void main() {
	vec3 objectColor = vec3(1.0, 0.0, 0.0);
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * vec3(0.0, 0.0, 0.0);

	float diff = max(dot(inNorm, vec3(0.85, 0.0, 0.0)), 0.0);
	vec3 diffuse = diff * lightColor;
	
    outColor = vec4(diffuse, 1.0);
}