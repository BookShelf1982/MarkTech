#pragma once
#include <PrimitiveTypes.h>

namespace MarkTech
{
	struct PackageMetadata
	{
		U32 id;
		U32 entryCount;
	};

	enum class EntryType
	{
		ANSI
	};

	struct BlobEntry
	{
		U64 entryId;
		EntryType entryType;
		U64 entryOffset;
		U64 entrySize;
	};

	struct PackageEntry
	{
		U64 entryId;
		EntryType entryType;
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
	void WritePackageToFile(Package* pPackage, const char* pFilepath);
}