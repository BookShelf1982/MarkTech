#pragma once
#include <vector>
#include "Vectors.h"
namespace MarkTech
{
	struct MVertex
	{
		MVertex(float x, float y, float z, float u, float v) : Position(x, y, z), TexCoords(u, v) {}

		MVector3 Position;
		MVector2 TexCoords;
	};
}