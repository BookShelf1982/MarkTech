#include "Package.h"
#include <File.h>
#include <MemoryArena.h>
#include <random>

namespace MarkTech
{
	void AddPackageEntry(Package* pPackage, PackageEntry entry)
	{
		// increment the entry count
		pPackage->metadata.entryCount++;

		// TODO: add the package entry in the entry linked list
		if (!pPackage->m_pEntryList)
		{
			pPackage->m_pEntryList = (PackageEntry*)AllocFromMemoryArena(sizeof(PackageEntry));
			*pPackage->m_pEntryList = entry;
			return;
		}

		PackageEntry* pEntry = pPackage->m_pEntryList;
		while (pEntry->pNext != nullptr)
		{
			pEntry = pEntry->pNext;
		}

		pEntry->pNext = (PackageEntry*)AllocFromMemoryArena(sizeof(PackageEntry));
		*pEntry->pNext = entry;
	}

	void WritePackageToFile(Package* pPackage, const char* pName)
	{
		// Open file
		char path[512] = "";
		strcpy(path, pName);
		AddExtension(path, ".mpk");
		File outputFile = FOpen(path, FileAccessType::WRITE);
		if (!outputFile.isOpened)
			return;

		// Write package metadata
		std::random_device rd;
		std::uniform_int_distribution<U32> dist;

		pPackage->metadata.id = dist(rd); // Generate a random resource id
		FWrite(&outputFile, (char*)&pPackage->metadata, sizeof(PackageMetadata));

		// Write package entries to file
		U64 offsetToEntryData = (sizeof(PackageMetadata) + (pPackage->metadata.entryCount * sizeof(BlobEntry)));
		PackageEntry* pEntry = pPackage->m_pEntryList;

		while (pEntry != nullptr)
		{
			BlobEntry entry = {};
			entry.entryId = pEntry->entryId;
			entry.entrySize = pEntry->entrySize;
			entry.entryType = pEntry->entryType;
			entry.entryOffset = offsetToEntryData;

			offsetToEntryData += entry.entrySize;

			FWrite(&outputFile, (char*)&entry, sizeof(BlobEntry));

			pEntry = pEntry->pNext;
		}

		// Write package entry data to file
		pEntry = pPackage->m_pEntryList;
		while (pEntry != nullptr)
		{
			FWrite(&outputFile, (char*)pEntry->pData, pEntry->entrySize);
			pEntry = pEntry->pNext;
		}

		// Close file
		FClose(&outputFile);
	}
}