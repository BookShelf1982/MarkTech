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

mat3 scale = mat3(
	vec3(100.0, 0.0, 0.0),
	vec3(0.0, 100.0, 0.0),
	vec3(0.0, 0.0, 1.0)
);

mat3 translate = mat3(
	vec3(1.0, 0.0, 0.0),
	vec3(0.0, 1.0, 0.0),
	vec3(350.0, 100.0, 1.0)
);

mat3 transform = translate * scale;
/*mat3(
	vec3(100.0, 0.0, 0.0),
	vec3(0.0, 100.0, 0.0),
	vec3(350.0, 100.0, 1.0)
);*/

const float r = 1280.0;
const float l = 0.0;
const float t = 720.0;
const float b = 0.0;

mat3 projection = mat3(
	vec3( 2.0 / (r - l), 0.0, 0.0),
	vec3( 0.0, 2.0 / (t - b), 0.0),
	vec3( -(r + l) / (r - l), -(t + b) / (t - b), 1.0)
);

layout(location = 0) out vec2 outTcoord;
layout(location = 1) out flat vec3 outVert;
layout(location = 2) out mat3 outMatrix;

layout(set = 0, binding = 2) uniform UniformBufferObject {
	mat3 transformMatrix;
} ubo;

void main() {
    //gl_Position = vec4(projection * transform * vec3(positions[gl_VertexIndex], 1.0), 1.0);
    gl_Position = vec4(ubo.transformMatrix * vec3(positions[gl_VertexIndex], 1.0), 1.0);
	outTcoord = tcoords[gl_VertexIndex];
	outMatrix = projection * transform;
	outVert = outMatrix * vec3(positions[gl_VertexIndex], 1.0);
}