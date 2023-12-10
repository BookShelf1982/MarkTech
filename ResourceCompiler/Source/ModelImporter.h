#pragma once
#include <stdint.h>
#include "OBJ_Loader\OBJ_Loader.h"

struct Vector3
{
	Vector3() {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

	float x, y, z;
};

struct Vector2
{
	Vector2() {}
	Vector2(float x, float y) : x(x), y(y) {}

	float x, y;
};

struct Vertex
{
	Vector3 pos;
	Vector2 tex;
};

extern "C"
{
	__declspec(dllexport) int LoadModel(const char* filepath, const char* output);
}