#pragma once
#include <PrimitiveTypes.h>
#include <AssetTypes.h>

namespace MarkTech
{
	struct AssetTableEntry
	{
		char assetFilepath[256];
		U32 assetId;
		AssetType type;
		U64 assetSize;
	};

	struct AssetTable
	{
		AssetTableEntry* pEntries;
		U32 entryCount;
	};

	U32 GetAssetListCount(const char* pAssetList);
	AssetTable CreateAssetTableFromAssetList(const char* pFilepath);
	void FreeAssetTable(AssetTable* pTable);
}