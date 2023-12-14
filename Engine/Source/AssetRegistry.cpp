#include "AssetRegistry.h"
#include "Configs.h"
#include "Shader.h"
#include "MModel.h"
#include "Texture.h"
#include <fstream>

namespace MarkTech
{
	CAssetHandle::CAssetHandle()
		:m_pAsset(nullptr), m_nAssetId(0)
	{
	}

	void CAssetHandle::Initialize(uint64_t assetId)
	{
		m_nAssetId = assetId;
		m_pAsset = GetLevel()->GetAssetRegistry()->FindAssetById(m_nAssetId);
	}

	CAssetObject* CAssetHandle::GetAssetDataPtr()
	{
		if (m_pAsset == nullptr || m_pAsset->m_nAssetId != m_nAssetId)
			return GetLevel()->GetAssetRegistry()->FindAssetById(m_nAssetId);
		else
			return m_pAsset;
	}

	CAssetRegistry::CAssetRegistry()
	{
	}

	CAssetRegistry::~CAssetRegistry()
	{
		for (int i = 0; i < m_RegisteredAssets.GetSize(); i++)
		{
			m_RegisteredAssets.c_arr()[i]->Release();
		}
	}

	uint64_t CAssetRegistry::LoadAsset(const char* filepath, EAssetType type)
	{
		switch (type)
		{
		case MUnknown:
		{
			return 0;
		}break;
		case MTexture:
		{
			char path[256];
			strcpy(path, MGameInfo::GetGameInfo()->szContentPath);
			strcat(path, filepath);
			uint64_t assetId = GetAssetId(path);
			CAssetObject* asset = FindAssetById(assetId);
			if (asset == nullptr)
			{
				m_RegisteredAssets.Push(LoadTexture(path));
			}
			return assetId;
		}break;
		case MModel:
		{
			char path[256];
			strcpy(path, MGameInfo::GetGameInfo()->szContentPath);
			strcat(path, filepath);
			uint64_t assetId = GetAssetId(path);
			CAssetObject* asset = FindAssetById(assetId);
			if (asset == nullptr)
			{
				m_RegisteredAssets.Push(LoadModel(path));
			}
			return assetId;
		}break;
		case MShader:
		{
			char path[256];
			strcpy(path, MGameInfo::GetGameInfo()->szShaderPath);
			strcat(path, filepath);
			uint64_t assetId = GetAssetId(path);
			CAssetObject* asset = FindAssetById(assetId);
			if (asset == nullptr)
			{
				m_RegisteredAssets.Push(LoadShader(path));
			}
			return assetId;
		}break;
		case MMaterial:
		{
			return 0;
		}break;
		}
		return 0;
	}

	CAssetObject* CAssetRegistry::FindAssetById(uint64_t id)
	{
		for (int i = 0; i < m_RegisteredAssets.GetSize(); i++)
		{
			if (m_RegisteredAssets.c_arr()[i]->m_nAssetId == id)
			{
				return m_RegisteredAssets.c_arr()[i];
			}
		}
		return nullptr;
	}

	uint64_t CAssetRegistry::GetAssetId(const char* filepath)
	{
		uint64_t id = 0;
		std::fstream file;
		file.open(filepath, std::ios::in | std::ios::binary);

		if (!file.is_open())
			return 0;

		file.seekg(0, file.beg);
		file.read((char*)&id, sizeof(uint64_t));
		file.close();
		return id;
	}

	CAssetObject::CAssetObject()
		: m_nAssetId(0)
	{
	}

	CAssetObject::~CAssetObject()
	{
	}

	void CAssetObject::Release()
	{
	}
}