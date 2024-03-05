#pragma once
#include "Core.h"
#include "Vectors.h"

#define MAX_LOADED_MODELS 1024
#define MAX_LOADED_SHADERS 2048

struct DummyVert
{
	MVector3 pos;
	MVector3 norm;
	MVector2 tcoords;
};

struct MAssetHandle
{
	uint64_t nAssetId;
};

struct MModelAsset
{
	MModelAsset();
	~MModelAsset();

	void Release();

	uint64_t m_nId;
	char* m_pGeoData;
	size_t m_nNumVerts;
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

	CMemoryPool m_AssetMemoryPool;
};
