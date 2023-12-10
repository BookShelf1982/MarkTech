#include "ModelImporter.h"
#include <iostream>
#include <fstream>
#include <string>
#include <random>

int LoadModel(const char* filepath, const char* output)
{
	objl::Loader loader;
	if (!loader.LoadFile(filepath))
	{
		std::cout << "Failed to find obj.\nPress enter to exit...\n";
		std::cin.get();
		return -1;
	}

	Vertex* verts = new Vertex[3644];
	uint32_t* inds = new uint32_t[loader.LoadedIndices.size()];

	for (size_t i = 0; i < 3644; i++)
	{
		Vector3 pos = Vector3(
			loader.LoadedVertices[i].Position.X,
			loader.LoadedVertices[i].Position.Y,
			loader.LoadedVertices[i].Position.Z);

		Vector2 tc = Vector2(loader.LoadedVertices[i].TextureCoordinate.X, loader.LoadedVertices[i].TextureCoordinate.Y);

		Vertex vert;
		vert.pos = pos;
		vert.tex = tc;

		verts[i] = vert;
	}

	/*for (size_t i = 0; i < loader.LoadedIndices.size(); i++)
	{
		inds[i] = loader.LoadedIndices[i];
	}*/

	std::fstream model;
	model.open(output, std::ios::out | std::ios::binary);
	if (!model.is_open())
	{
		std::cout << "Failed to find output path.\nPress enter to exit...\n";
		std::cin.get();
		delete[] inds;
		delete[] verts;
		return -1;
	}

	size_t nVerts = 3644;
	size_t nInds = loader.LoadedIndices.size();

	std::random_device rd;
	std::uniform_int_distribution<uint64_t> dist;
	uint64_t assetId = dist(rd);

	std::cout << "Asset Id: " << assetId << "\n";

	model.write((char*)&assetId, sizeof(uint64_t));
	model.write((char*)&nVerts, sizeof(size_t));
	model.write((char*)&nInds, sizeof(size_t));
	model.write((char*)verts, sizeof(Vertex) * nVerts);
	model.write((char*)inds, sizeof(uint32_t) * nInds);

	model.close();

	delete[] inds;
	delete[] verts;
	return 0;
}
