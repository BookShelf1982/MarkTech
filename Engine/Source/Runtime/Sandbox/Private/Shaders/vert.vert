#version 450

vec2 positions[6] = vec2[](
    vec2(0.5, -0.5),
    vec2(-0.5, 0.5),
    vec2(-0.5, -0.5),
	vec2(-0.5, 0.5),
	vec2(0.5, -0.5),
	vec2(0.5, 0.5)
);

vec2 tcoords[6] = vec2[](
	vec2(1.0, 0.0),
	vec2(0.0, 1.0),
	vec2(0.0, 0.0),
	vec2(0.0, 1.0),
	vec2(1.0, 0.0),
	vec2(1.0, 1.0)
);

layout(location = 0) out vec2 outTcoord;

layout(set = 0, binding = 1) uniform PerFrameInfo {
	mat3 projection;
} perFrameInfo;

layout(set = 2, binding = 0) uniform PerSpriteinfo {
	mat3 world;
} perSpriteInfo;

void main() {
    gl_Position = vec4((perFrameInfo.projection * perSpriteInfo.world) * vec3(positions[gl_VertexIndex], 1.0), 1.0);
	outTcoord = tcoords[gl_VertexIndex];
}