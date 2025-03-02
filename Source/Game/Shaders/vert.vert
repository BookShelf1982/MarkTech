#version 460

const vec2 poslist[3] = vec2[](
	vec2(-0.5, -0.5),
	vec2(0, 0.5),
	vec2(0.5, -0.5)
);

layout(binding = 0) uniform OriginBuffer {
	vec2 origin;
} ubo;

void main()
{
	mat3 translate = mat3(
		vec3(1.0, 0.0, ubo.origin.x),
		vec3(0.0, 1.0, ubo.origin.y),
		vec3(0.0, 0.0, 1.0)
	);
	
	gl_Position = vec4(vec3(poslist[gl_VertexIndex], 1.0) * translate, 1.0);
}