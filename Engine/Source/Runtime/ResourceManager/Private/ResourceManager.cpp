#include "ResourceManager.h"
#include <Memory\MemoryManager.h>
#include <String\DebugStringManager.h>
#include <File.h>

namespace MarkTech
{
	ResourceManager* ResourceManager::m_gpResourceManager = nullptr;

	ResourceManager::ResourceManager()
		:m_AssetAllocator(nullptr, 0), m_PackageCount(0)
	{
		if (!m_gpResourceManager)
			m_gpResourceManager = this;
	}

	ResourceManager::~ResourceManager()
	{
	}

	void ResourceManager::Init(U64 sizeInBytes)
	{
		m_AssetAllocator = StackAllocator(sizeInBytes, 1);
	}

	void ResourceManager::Shutdown()
	{
		m_LookupList.~Dictionary();
		m_PackageFilename.~Dictionary();
		m_ResourcePackageOwnership.~Dictionary();
		m_AssetAllocator.Clear();
	}

	void ResourceManager::LoadPackage(const char* filepath)
	{
		// Open file
		File input(filepath, FileAccessType::READ);
		if (!input.IsOpen())
			return;

		// Add package to package filename table
		U64 size = strlen(filepath);
		char* pStr = (char*)DebugStringManager::AllocStringDbg(size);
		strcpy(pStr, filepath);
		m_PackageFilename.Insert(m_PackageCount, pStr);

		// Read metadata
		PackageMetadata metadata = {};
		input.Read((char*)&metadata, sizeof(PackageMetadata));

		// Read package entries
		for (U32 i = 0; i < metadata.entryCount; i++)
		{
			BlobEntry entry = {};
			input.Read((char*)&entry, sizeof(BlobEntry));
			
			LookupData data = {};
			data.pData = nullptr;
			data.size = entry.entrySize;
			data.type = entry.entryType;

			m_LookupList.Insert(entry.entryId, data);
			m_ResourcePackageOwnership.Insert(entry.entryId, m_PackageCount);
		}

		m_PackageCount++;
	}

	bool ResourceManager::LoadResourceWithId(U64 id)
	{
		// Check if id even exists
		if (!m_LookupList.Has(id))
			return false;

		// See if resource is already loaded
		if (m_LookupList.Get(id).pData != nullptr)
			return true;

		// Load resource
		if (LoadResourceFromPackage(id, m_ResourcePackageOwnership.Get(id)))
			return true;

		return false;
	}

	bool ResourceManager::LoadResourceFromPackage(U64 resource, U8 package)
	{
		// Opem package from id
		File file(m_PackageFilename.Get(package), FileAccessType::READ);
		if (!file.IsOpen())
			return false;

		// Read metadata
		PackageMetadata metadata = {};
		file.Read((char*)&metadata, sizeof(PackageMetadata));

		// Read package entries
		U64 byteOffset = 0;
		for (U32 i = 0; i < metadata.entryCount; i++)
		{
			BlobEntry entry = {};
			file.Read((char*)&entry, sizeof(BlobEntry));
			if (entry.entryId == resource)
			{
				byteOffset = entry.entryOffset;
				break;
			}
		}

		if (byteOffset == 0)
			return false;
		
		// Go to offset
		file.Seek((byteOffset), SeekOrigin::BEGINING);

		LookupData data = m_LookupList.Get(resource);
		data.pData = (U8*)m_AssetAllocator.Alloc(data.size);

		file.Read((char*)data.pData, data.size);

		m_LookupList.Modify(resource, data);

		return true;
	}

	void ResourceManager::LoadResourcePackage(const char* filepath)
	{
		m_gpResourceManager->LoadPackage(filepath);
	}

	bool ResourceManager::LoadResource(U64 id)
	{
		return m_gpResourceManager->LoadResourceWithId(id);
	}
}