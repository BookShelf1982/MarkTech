#include "AssetRegistry.h"
#include <fstream>
#include <iostream>

MModelAsset::MModelAsset()
	:m_nId(0), m_pGeoData(nullptr), m_nNumVerts(0), m_nNumInds(0)
{
}

MModelAsset::~MModelAsset()
{
}

void MModelAsset::Release()
{
}

CAssetRegistry::CAssetRegistry()
	:m_nModelsCurrentSize(0), m_nShadersCurrentSize(0)
{
	m_pModels = new MModelAsset[MAX_LOADED_MODELS];
	m_pShaders = new MShaderAsset[MAX_LOADED_SHADERS];
}

CAssetRegistry::~CAssetRegistry()
{
	delete[] m_pModels;
	delete[] m_pShaders;
}

void CAssetRegistry::InitAssetRegistry(CMemoryPool* memoryPool)
{
	m_pMemoryPoolRef = memoryPool;
}

MShaderAsset::MShaderAsset()
	:m_nId(0), m_nShaderBytecodeSize(0), m_pShaderBytecode(nullptr)
{
}

MShaderAsset::~MShaderAsset()
{
}

void MShaderAsset::Release()
{
}

void CAssetRegistry::DestroyAssetRegistry()
{
	for (size_t i = 0; i < m_nModelsCurrentSize; i++)
	{
		m_pModels[i].Release();
	}
	for (size_t i = 0; i < m_nShadersCurrentSize; i++)
	{
		m_pShaders[i].Release();
	}
}

uint64_t CAssetRegistry::LoadModelAsset(String path)
{
	uint64_t id;
	size_t nNumVerts;
	size_t nNumInds;

	std::fstream file;
	file.open(path.c_str(), std::ios::in | std::ios::binary);
	if (!file.is_open())
		return 0;

	if(!file.read((char*)&id, sizeof(uint64_t)))
		return 0;

	if (!file.read((char*)&nNumVerts, sizeof(size_t)))
		return 0;

	if (!file.read((char*)&nNumInds, sizeof(size_t)))
		return 0;

	char* pDat = m_pMemoryPoolRef->AllocateArray<char>(nNumVerts * sizeof(DummyVert) + nNumInds * sizeof(uint32_t));
	file.read((char*)pDat, sizeof(DummyVert) * nNumVerts + sizeof(uint32_t) * nNumInds);
	file.close();

	m_pModels[m_nModelsCurrentSize].m_nId = id;
	m_pModels[m_nModelsCurrentSize].m_nNumVerts = nNumVerts;
	m_pModels[m_nModelsCurrentSize].m_nNumInds = nNumInds;
	m_pModels[m_nModelsCurrentSize].m_pGeoData = pDat;

	m_nModelsCurrentSize++;
	return id;
}

uint64_t CAssetRegistry::LoadShaderAsset(String path)
{
	uint64_t id = 0;
	size_t nShaderDataSize = 0;

	std::fstream InputFile;
	InputFile.open(path.c_str(), std::ios::in | std::ios::binary);

	if (!InputFile.is_open())
		return false;

	InputFile.read((char*)&id ,sizeof(uint64_t));
	InputFile.read((char*)&nShaderDataSize, sizeof(size_t));
	char* pData = m_pMemoryPoolRef->AllocateArray<char>(nShaderDataSize);
	InputFile.read((char*)pData, nShaderDataSize);
	InputFile.close();

	m_pShaders[m_nShadersCurrentSize].m_nId = id;
	m_pShaders[m_nShadersCurrentSize].m_nShaderBytecodeSize = nShaderDataSize;
	m_pShaders[m_nShadersCurrentSize].m_pShaderBytecode = pData;

	m_nShadersCurrentSize++;
	return id;
}

MModelAsset* CAssetRegistry::GetModelAsset(uint64_t assetId)
{
	for(size_t i = 0; i < m_nModelsCurrentSize; i++)
	{
		if (m_pModels[i].m_nId == assetId)
		{
			return &m_pModels[i];
		}
	}
	return nullptr;
}

MShaderAsset* CAssetRegistry::GetShaderAsset(uint64_t assetId)
{
	for (size_t i = 0; i < m_nShadersCurrentSize; i++)
	{
		if (m_pShaders[i].m_nId == assetId)
		{
			return &m_pShaders[i];
		}
	}
	return nullptr;
}