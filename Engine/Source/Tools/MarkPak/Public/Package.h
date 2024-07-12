#pragma once
#include <PrimitiveTypes.h>
#include <DSA\LinkedList.h>

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
	};

	class Package
	{
	public:
		Package();
		~Package();

		void AddPackageEntry(PackageEntry entry);
		void WritePackageToFile(const char* filepath);
	private:
		PackageMetadata metadata;
		LinkedList<PackageEntry> m_EntryList;
	};
}