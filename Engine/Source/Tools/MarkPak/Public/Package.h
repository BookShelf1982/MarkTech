#pragma once
#include <PrimitiveTypes.h>
#include "AssetTypes.h"

namespace MarkTech
{
	struct PackageMetadata
	{
		U32 id;
		U32 entryCount;
	};

	struct BlobEntry
	{
		U64 entryId;
		AssetType entryType;
		U64 entryOffset;
		U64 entrySize;
	};

	struct PackageEntry
	{
		U64 entryId;
		AssetType entryType;
		void* pData;
		U64 entrySize;
		PackageEntry* pNext;
	};

	struct Package
	{
		PackageMetadata metadata;
		PackageEntry* m_pEntryList;
	};

	void AddPackageEntry(Package* pPackage, PackageEntry entry);
	void WritePackageToFile(Package* pPackage, const char* pName);
}