#pragma once
#include <vector>
#include "Vectors.h"

struct MVertex
{
	MVertex(MVector4 Pos, MVector3 Normal, MVector2 TCoord);

	MVector4 Position;
	MVector3 Normal;
	MVector2 TCoord;
};

class CMModel
{
public:
	CMModel()
	{

	}
	~CMModel();

	std::vector<MVertex> vertices;

	std::vector<unsigned int> indices;
};

