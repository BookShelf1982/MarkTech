#include "ModelImporter.h"
#include <fstream>
#include <random>


int LoadModel(const char* filepath, const char* output)
{
	FbxManager* pFbxManager = FbxManager::Create();

	FbxIOSettings* pIos = FbxIOSettings::Create(pFbxManager, IOSROOT);
	pFbxManager->SetIOSettings(pIos);

	FbxImporter* pFbxImporter = FbxImporter::Create(pFbxManager, "");

	if (!pFbxImporter->Initialize(filepath, -1, pFbxManager->GetIOSettings()))
	{
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error reported: %s\n\n", pFbxImporter->GetStatus().GetErrorString());
		pFbxImporter->Destroy();
		pFbxManager->Destroy();
		return 1;
	}

	printf("Sucessfully imported mesh!\n");

	FbxScene* pFbxScene = FbxScene::Create(pFbxManager, "MainScene");
	pFbxImporter->Import(pFbxScene);
	pFbxImporter->Destroy();

	fbxsdk::FbxMesh* pMesh = pFbxScene->GetRootNode()->GetChild(0)->GetMesh();
	if (pMesh == nullptr)
	{
		printf("Failed to get mesh data.\n");
		pFbxManager->Destroy();
		return 1;
	}
	
	if (!pMesh->IsTriangleMesh())
	{
		printf("Triangulated meshes are only supported in Resource Compiler.\nMesh is now being triangulated.\n");
		FbxGeometryConverter converter(pFbxManager);
		if (!converter.Triangulate(pFbxScene, true))
		{
			printf("Failed to triangulate mesh.\n");
			return 1;
		}
		printf("Mesh triangulation succeded!\n");
	}

	pMesh = pFbxScene->GetRootNode()->GetChild(0)->GetMesh();
	size_t totalVerts = pMesh->GetControlPointsCount();
 	size_t totalFace = pMesh->GetPolygonCount();
	MVertex* pVertArray = new MVertex[totalVerts];
	uint32_t* pIndArray = nullptr;
	printf("Mesh vertex count: %Iu\n", totalVerts);
	printf("Mesh face count: %Iu\n", totalFace);

	// Mesh Formatting
	FbxVector4* vert_data = pMesh->GetControlPoints();

	// Get vertices
	for (size_t i = 0; i < totalVerts; i++)
	{
		pVertArray[i].pos = MVector3(vert_data[i].mData[0], vert_data[i].mData[1], vert_data[i].mData[2]);
		printf("vert pos: %f, %f, %f\n", vert_data[i].mData[0], vert_data[i].mData[1], vert_data[i].mData[2]);
	}
	
	// Get indices size
	size_t indsCount = 0;
	for (size_t i = 0; i < totalFace; i++)
	{
		for (size_t j = 0; j < pMesh->GetPolygonSize(i); j++)
		{
			indsCount++;
		}
	}

	pIndArray = new uint32_t[indsCount];

	// Get indices
	uint32_t indCounter = 0;
	for (size_t i = 0; i < totalFace; i++)
	{
		uint32_t faceSize = pMesh->GetPolygonSize(i);
		for (size_t j = 0; j < faceSize; j++)
		{
			pIndArray[indCounter] = pMesh->GetPolygonVertex(i, j);
			indCounter++;
		}
	}

	// Export mmdl file
	std::fstream foutput;
	foutput.open(output, std::ios::out | std::ios::binary);
	if (!foutput.is_open())
	{
		printf("Failed to find output path.\n");
		pFbxManager->Destroy();
		return 1;
	}

	std::random_device rd;
	std::uniform_int_distribution<uint64_t> dist;
	uint64_t id = dist(rd);

	printf("Asset Id: %Iu", id);

	foutput.write((char*)&id, sizeof(uint64_t));
	foutput.write((char*)&totalVerts, sizeof(size_t));
	foutput.write((char*)&indsCount, sizeof(size_t));
	foutput.write((char*)pVertArray, sizeof(MVertex) * totalVerts);
	foutput.write((char*)pIndArray, sizeof(uint32_t) * indsCount);
	foutput.close();

	delete[] pVertArray;
	delete[] pIndArray;

	pFbxManager->Destroy();
	return 0;
}
