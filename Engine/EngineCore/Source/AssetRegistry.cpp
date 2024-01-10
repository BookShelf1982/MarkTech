#include "AssetRegistry.h"
#include <fstream>
#include <iostream>

MModelAsset::MModelAsset()
	:m_nId(0), m_pVertData(nullptr), m_nNumVerts(0), m_pIndData(nullptr), m_nNumInds(0)
{
}

MModelAsset::~MModelAsset()
{
}

void MModelAsset::Release()
{
	if (m_pVertData)
		delete[] m_pVertData;
	if (m_pIndData)
		delete[] m_pIndData;
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

MShaderAsset::MShaderAsset()
	:m_nId(0), m_nShaderBytecodeSize(0), m_pShaderBytecode(nullptr)
{
}

MShaderAsset::~MShaderAsset()
{
}

void MShaderAsset::Release()
{
	if(m_pShaderBytecode)
		delete[] m_pShaderBytecode;
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

	char* pVertData = new char[nNumVerts * sizeof(DummyVert)];
	char* pIndData = new char[nNumInds * sizeof(uint32_t)];
	file.read((char*)pVertData, sizeof(DummyVert) * nNumVerts);
	file.read((char*)pIndData, sizeof(uint32_t) * nNumInds);
	file.close();

	m_pModels[m_nModelsCurrentSize].m_nId = id;
	m_pModels[m_nModelsCurrentSize].m_nNumVerts = nNumVerts;
	m_pModels[m_nModelsCurrentSize].m_nNumInds = nNumInds;
	m_pModels[m_nModelsCurrentSize].m_pVertData = pVertData;
	m_pModels[m_nModelsCurrentSize].m_pIndData = pIndData;

	m_nModelsCurrentSize++;
	return id;
}

uint64_t CAssetRegistry::LoadShaderAsset(String path)
{
	uint64_t id = 0;
	size_t nVertShaderDataSize = 0;
	size_t nShaderSourceDataSize = 0;

	std::fstream InputFile;
	InputFile.open(path.c_str(), std::ios::in | std::ios::binary);

	if (!InputFile.is_open())
		return false;

	InputFile.read((char*)&id ,sizeof(uint64_t));
	InputFile.read((char*)&nShaderSourceDataSize, sizeof(size_t));
	InputFile.read((char*)&nVertShaderDataSize, sizeof(size_t));
	InputFile.seekg(nShaderSourceDataSize, std::ios::_Seekcur);
	char* pVertShaderData = new char[nVertShaderDataSize];
	InputFile.read((char*)pVertShaderData, nVertShaderDataSize);
	InputFile.close();

	m_pShaders[m_nShadersCurrentSize].m_nId = id;
	m_pShaders[m_nShadersCurrentSize].m_nShaderBytecodeSize = nVertShaderDataSize;
	m_pShaders[m_nShadersCurrentSize].m_pShaderBytecode = pVertShaderData;

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