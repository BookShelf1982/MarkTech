#pragma once
#include <PrimitiveTypes.h>
#include "AssetTypes.h"

namespace MarkTech
{
	struct AssetTableEntry
	{
		U64 id;
		char* pPath;
	};

	struct AssetTable
	{
		U64 entryCount;
		AssetTableEntry* pEntries;
	};

	AssetTable CreateAssetTable(const char* pPath);
	AssetType EvaluateAssetType(char* pPath);
}