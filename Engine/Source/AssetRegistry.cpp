#include "AssetRegistry.h"
#include "Configs.h"
#include <fstream>

namespace MarkTech
{
	CAssetHandle::CAssetHandle()
		:m_pAsset(nullptr)
	{
	}

	void CAssetHandle::Initialize(MAssetData* asset)
	{
		m_pAsset = asset;
	}

	CAssetRegistry::CAssetRegistry()
	{
	}

	CAssetRegistry::~CAssetRegistry()
	{
	}

	MAssetData* CAssetRegistry::LoadAsset(const char* filepath, EAssetType type)
	{
		switch (type)
		{
		case MUnknown:
		{
			return nullptr;
		}break;
		case MTexture:
		{
			char path[256];
			strcpy(path, MGameInfo::GetGameInfo()->szContentPath);
			strcat(path, filepath);
			uint64_t assetId = GetAssetId(path);
			MAssetData* asset = FindAssetById(assetId);
			if (asset == nullptr)
			{
				m_RegisteredAssets.Push(LoadTexture(path));
				return FindAssetById(assetId);
			}
			return asset;
		}break;
		case MModel:
		{
			return nullptr;
		}break;
		case MShader:
		{
			return nullptr;
		}break;
		case MMaterial:
		{
			return nullptr;
		}break;
		}
		return nullptr;
	}

	MAssetData* CAssetRegistry::FindAssetById(uint64_t id)
	{
		for (int i = 0; i < m_RegisteredAssets.GetSize(); i++)
		{
			if (m_RegisteredAssets.c_arr()[i].nId == id)
			{
				return &m_RegisteredAssets.c_arr()[i];
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

	MAssetData CAssetRegistry::LoadTexture(const char* filepath)
	{
		MAssetData tempAsset = {};
		tempAsset.Type = MTexture;
		std::fstream file;
		file.open(filepath, std::ios::in | std::ios::binary);

		file.read((char*)&tempAsset.nId, sizeof(uint64_t));
		file.seekg((std::streamoff)sizeof(uint64_t), file.end);
		std::streampos length = file.tellg();
		tempAsset.nDataSize = length;
		tempAsset.pData = new char[length];
		file.seekg((std::streamoff)sizeof(uint64_t), file.beg);
		file.read((char*)tempAsset.pData, length);
		file.close();

		return tempAsset;
	}


}