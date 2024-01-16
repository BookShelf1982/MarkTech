#pragma once
#include "Core.h"
#include "Vectors.h"

#define MAX_LOADED_MODELS 4096
#define MAX_LOADED_SHADERS 4096

struct DummyVert
{
	MVector3 pos;
	MVector3 norm;
	MVector2 tcoords;
};

struct MModelAsset
{
	MModelAsset();
	~MModelAsset();

	void Release();

	uint64_t m_nId;
	char* m_pVertData;
	size_t m_nNumVerts;
	char* m_pIndData;
	size_t m_nNumInds;
};


struct MShaderAsset
{
	MShaderAsset();
	~MShaderAsset();

	void Release();

	uint64_t m_nId;
	char* m_pShaderBytecode;
	size_t m_nShaderBytecodeSize;
};

class CAssetRegistry
{
public:
	CAssetRegistry();
	~CAssetRegistry();

	uint64_t LoadModelAsset(String path);
	MModelAsset* GetModelAsset(uint64_t assetId);
	uint64_t LoadShaderAsset(String path);
	MShaderAsset* GetShaderAsset(uint64_t assetId);

	void DestroyAssetRegistry();

private:
	MModelAsset* m_pModels;
	MShaderAsset* m_pShaders;
	size_t m_nShadersCurrentSize;
	size_t m_nModelsCurrentSize;
};