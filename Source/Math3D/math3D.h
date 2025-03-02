#ifndef MATH3D_H
#define MATH3D_H

#include <stdalign.h>

typedef struct {
	alignas(16) float v[4];
} vec3f_t;

typedef struct {
	alignas(16) float v[4];
} vec4f_t;

typedef struct {
	alignas(16) float m[16];
} mat4x4f_t;

typedef mat4x4f_t mat4f_t;

vec3f_t add3f(vec3f_t a, vec3f_t b);
vec3f_t sub3f(vec3f_t a, vec3f_t b);
vec3f_t muls3f(vec3f_t a, float s);
float dot3f(vec3f_t a, vec3f_t b);
vec3f_t cross3f(vec3f_t a, vec3f_t b);

vec4f_t add4f(vec4f_t a, vec4f_t b);
vec4f_t sub4f(vec4f_t a, vec4f_t b);

mat4x4f_t mul4x4f(mat4x4f_t a, mat4x4f_t b);
mat4x4f_t perspective4x4f(float aspect, float fov, float zNear, float zFar);

#endif