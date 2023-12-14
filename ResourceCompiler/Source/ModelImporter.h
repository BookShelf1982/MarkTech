#pragma once
#include <stdint.h>
#include "OBJ_Loader\OBJ_Loader.h"

struct MVector3
{
	MVector3() {}
	MVector3(float x, float y, float z) : x(x), y(y), z(z) {}

	float x, y, z;
};

struct MVector2
{
	MVector2() {}
	MVector2(float x, float y) : x(x), y(y) {}

	float x, y;
};

struct MVertex
{
	MVertex() {}
	MVertex(float x, float y, float z, float nx, float ny, float nz, float u, float v) : pos(x, y, z), norm(nx, ny, nz), tex(u, v) {}

	MVector3 pos;
	MVector3 norm;
	MVector2 tex;
};

extern "C"
{
	__declspec(dllexport) int LoadModel(const char* filepath, const char* output);
}