#pragma once
#include <vector>
#include "Vectors.h"

class MVertex
{
	MVertex();

	MVector3 Position;
	MVector3 Normal;
	MVector2 TCoord;
};

class MModel
{
public:
	MModel()
	{

	}
	~MModel();

	void UploadVerticesToIA(std::vector<MVertex> Vertices);

	std::vector<MVertex> vertices;

	std::vector<unsigned int> indices;
};

