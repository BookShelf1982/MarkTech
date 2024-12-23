#version 450

vec2 positions[6] = vec2[](
    vec2(-1.0, -1.0),
    vec2(1.0, -1.0),
    vec2(-1.0, 1.0),
	vec2(-1.0, 1.0),
	vec2(1.0, -1.0),
	vec2(1.0, 1.0)
);

vec2 tcoords[6] = vec2[](
	vec2(0.0, 1.0),
	vec2(1.0, 1.0),
	vec2(0.0, 0.0),
	vec2(0.0, 0.0),
	vec2(1.0, 1.0),
	vec2(1.0, 0.0)
);

layout(location = 0) out vec2 outTcoord;
layout(set = 0, binding = 2) uniform UniformBufferObject {
	layout(row_major) mat4x4 transformMatrix;
} ubo;

void main() {
    gl_Position = vec4(positions[gl_VertexIndex], 1.0, 1.0) * ubo.transformMatrix;
	outTcoord = tcoords[gl_VertexIndex];
}