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
		std::cout << "Could not find obj file.\nPress enter to exit...\n";
		std::cin.get();
		return -1;
	}
	size_t nVerts = loader.LoadedVertices.size();
	size_t nInds = loader.LoadedIndices.size();

	MVertex* vert = new MVertex[nVerts];
	uint32_t* inds = new uint32_t[nInds];

	for (size_t i = 0; i < nVerts; i++)
	{
		vert[i] = {
			loader.LoadedVertices[i].Position.X,
			loader.LoadedVertices[i].Position.Y,
			loader.LoadedVertices[i].Position.Z,
			loader.LoadedVertices[i].Normal.X,
			loader.LoadedVertices[i].Normal.Y,
			loader.LoadedVertices[i].Normal.Z,
			loader.LoadedVertices[i].TextureCoordinate.X,
			loader.LoadedVertices[i].TextureCoordinate.Y };
	}

	for (size_t i = 0; i < nInds; i++)
	{
		inds[i] = loader.LoadedIndices[i];
	}

	std::string Path = output;
	std::fstream model;
	// If the file is not an .mmdl file return false
	if (Path.substr(Path.size() - 5, 5) != ".mmdl")
	{
		std::cout << "Output file is not an mmdl file.\nPress enter to exit...\n";
		std::cin.get();
		delete[] vert;
		delete[] inds;
		return -1;
	}

	model.open(output, std::ios::out | std::ios::binary);
	if (!model.is_open())
	{
		std::cout << "Failed to find output path.\nPress enter to exit...\n";
		std::cin.get();
		delete[] vert;
		delete[] inds;
		return -1;
	}

	std::random_device rd;
	std::uniform_int_distribution<uint64_t> dist;
	uint64_t assetId = dist(rd);

	std::cout << "Asset Id: " << assetId << "\n";

	model.write((char*)&assetId, sizeof(uint64_t));
	model.write((char*)&nVerts, sizeof(size_t));
	model.write((char*)&nInds, sizeof(size_t));
	model.write((char*)vert, sizeof(MVertex) * nVerts);
	model.write((char*)inds, sizeof(uint32_t) * nInds);

	model.close();

	std::cout << "Asset Export Sucess! \n";
	delete[] vert;
	delete[] inds;
	return 0;
}
