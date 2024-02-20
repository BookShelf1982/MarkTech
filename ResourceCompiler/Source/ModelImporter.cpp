#include "ModelImporter.h"
#include <fstream>
#include <random>

#define LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)

int LoadModel(const char* filepath, const char* output)
{
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(filepath, LOAD_FLAGS);
	if (!pScene)
	{
		printf("Failed to import model: '%s': '%s'\n", filepath, Importer.GetErrorString());
	}

	aiMesh* pMesh = pScene->mMeshes[0];

	size_t nNumVerts = pMesh->mNumVertices;
	size_t nNumFaces = pMesh->mNumFaces;
	size_t nNumIndices = pMesh->mNumFaces * 3;

	printf("Verts: %zu\nFaces: %zu\nInds: %zu\n", nNumVerts, nNumFaces, nNumIndices);

	MVertex* pVertArray = new MVertex[pMesh->mNumVertices];
	uint32_t* pIndArray = new uint32_t[pMesh->mNumFaces * 3];

	for (size_t i = 0; i < nNumVerts; i++)
	{
		pVertArray[i].pos = MVector3(pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z);
		pVertArray[i].norm = MVector3(pMesh->mNormals[i].x, pMesh->mNormals[i].y, pMesh->mNormals[i].z);
		if (pMesh->HasTextureCoords(0))
		{
			pVertArray[i].tex = MVector2(pMesh->mTextureCoords[0][i].x, pMesh->mTextureCoords[0][i].y);
		}
		else
		{
			pVertArray[i].tex = MVector2(0, 0);
		}
	}

	uint32_t indexCounter = 0;
	for (size_t i = 0; i < nNumFaces; i++)
	{
		for (uint32_t j = 0; j < pMesh->mFaces[i].mNumIndices; j++)
		{
			if (pMesh->mFaces[i].mNumIndices != 3)
			{
				printf("Mesh is not triangulated.");
				break;
			}
			pIndArray[indexCounter] = pMesh->mFaces[i].mIndices[j];
			indexCounter++;
		}
	}

	std::fstream outFile;
	outFile.open(output, std::ios::out | std::ios::binary);

	if (!outFile.is_open())
	{
		printf("Failed to fine output path.\n");
		return 1;
	}

	std::random_device rd;
	std::uniform_int_distribution<uint64_t> dist;
	uint64_t nAssetId = dist(rd);

	printf("Asset Id: %Iu\n", nAssetId);

	if (!outFile.write((char*)&nAssetId, sizeof(uint64_t)))
	{
		printf("Failed to write to file.");
		return 1;
	}
	if(!outFile.write((char*)&nNumVerts, sizeof(size_t)))
	{
		printf("Failed to write to file.");
		return 1;
	}
	if(!outFile.write((char*)&nNumIndices, sizeof(size_t)))
	{
		printf("Failed to write to file.");
		return 1;
	}
	outFile.write((char*)pIndArray, sizeof(uint32_t) * nNumIndices);
	outFile.write((char*)pVertArray, sizeof(MVertex) * nNumVerts);
	outFile.close();

	printf("Asset exported!\n");

	delete[] pVertArray;
	delete[] pIndArray;

	return 0;
}
