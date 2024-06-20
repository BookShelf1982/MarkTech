#include "Package.h"
#include <Memory\MemoryManager.h>
#include <File.h>

namespace MarkTech
{
	Package::Package()
		:metadata()
	{
	}

	Package::~Package()
	{
	}

	void Package::AddPackageEntry(PackageEntry entry)
	{
		metadata.entryCount++;
		metadata.blobSizeInBytes += entry.entrySize;
		m_EntryList.Insert(entry);
	}

	void Package::WritePackageToFile(const char* filepath)
	{
		// Open file
		File outputFile(filepath, FileAccessType::WRITE);
		if (!outputFile.IsOpen())
			return;

		// Write package metadata
		outputFile.Write((char*)&metadata, sizeof(PackageMetadata));

		// Write package entries to file
		U64 offsetToEntryData = (sizeof(PackageMetadata) + (metadata.entryCount * sizeof(BlobEntry)));
		for (auto it = m_EntryList.Begin(); it != nullptr; it++)
		{
			BlobEntry entry = {};
			entry.entryId = it->data.entryId;
			entry.entrySize = it->data.entrySize;
			entry.entryType = it->data.entryType;
			entry.entryOffset = offsetToEntryData;
			
			offsetToEntryData += entry.entrySize;

			outputFile.Write((char*)&entry, sizeof(BlobEntry));
		}

		// Write package entry data to file
		for (auto it = m_EntryList.Begin(); it != nullptr; it++)
		{
			outputFile.Write((char*)it->data.pData, it->data.entrySize);
		}

		// Close file
		outputFile.Close();
	}
}