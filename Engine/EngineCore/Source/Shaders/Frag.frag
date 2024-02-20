#version 450

layout(location = 0) in vec4 inNorm;
layout(location = 1) in vec2 inTCoord;

layout(location = 0) out vec4 outColor;

void main() {
	vec3 objectColor = vec3(1.0, 1.0, 0.0);
	vec3 ambient = objectColor + vec3(0.0, 0.31, 0.52);
	
	float reflectance = dot(normalize(inNorm.rgb), vec3(1.0, -1.0, 0.0));
	
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 diffuse = reflectance * lightColor;
	
	vec3 finalColor = inNorm.rgb;
	
    outColor = vec4(finalColor, 1.0);
}