#pragma once
#include "Core.h"
#include "Vectors.h"
namespace MarkTech
{
	struct MVertex
	{
		MVertex() {}
		MVertex(float x, float y, float z, float u, float v) : Position(x, y, z), TexCoords(u, v) {}

		MVector3 Position;
		MVector2 TexCoords;
	};

	MAKE_CTARRAY_COMPATIBLE(MVertex);
	MAKE_CTARRAY_COMPATIBLE(int);

	class MARKTECH_API CModel
	{
	public:
		CModel();
		~CModel();

		

	private:
		CTArray<MVertex> m_Vertices;
		CTArray<int> m_Indices;


	};


}