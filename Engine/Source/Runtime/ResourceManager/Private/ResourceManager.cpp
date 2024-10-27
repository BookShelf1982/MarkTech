#include "ResourceManager.h"
#include <File.h>

namespace MarkTech
{
	bool LoadPackage(ResourceManager* pManager, const char* pFilepath)
	{
		File file;
		FOpen(&file, pFilepath, FILE_ACCESS_READ);
		if (!file.isOpened)
			return false;

		U64 entryCount = 0;
		U32 packageFlags = 0;
		// Skip package signature
		U8 signatureLen = 0;
		FRead(&file, (char*)&signatureLen, 1);
		if (signatureLen != 0)
			FSeek(&file, signatureLen, SEEK_ORIGIN_CURRENT);

		FRead(&file, (char*)&entryCount, sizeof(U64));
		FRead(&file, (char*)&packageFlags, sizeof(U32));

		// if package flags has the string table flag then read string table
		if (packageFlags & 0x01)
		{
			U64 stringCount = 0;
			FRead(&file, (char*)&stringCount, sizeof(U32)); // Read the amount of string table entries
			for (U64 i = 0; i < stringCount; i++)
			{
				// Read entries
				StringTableEntry* pEntry = (StringTableEntry*)AllocFromPool(pManager->pStringTableAlloc);
				pEntry->pNext = nullptr;

				FRead(&file, (char*)&pEntry->assetId, sizeof(U32));
				U8 strLen = 0;
				FRead(&file, (char*)&strLen, sizeof(U8));
				FRead(&file, (char*)pEntry->assetName, strLen);
				pEntry->assetName[strLen] = 0;

				// add to list
				if (!pManager->pFirstStringTableEntry)
					pManager->pFirstStringTableEntry = pEntry;
				else
				{
					StringTableEntry* pLastEntry = pManager->pFirstStringTableEntry;
					while (pLastEntry->pNext != nullptr)
					{
						pLastEntry = pLastEntry->pNext;
					}

					pLastEntry->pNext = pEntry;
				}
			}
		}

		for (U32 i = 0; i < entryCount; i++)
		{
			ResourceEntry* pEntry = (ResourceEntry*)AllocFromPool(pManager->pResourceEntryAlloc);
			pEntry->pNext = nullptr;
			pEntry->pData = nullptr;
			
			FRead(&file, (char*)&pEntry->resourceId, sizeof(U32)); // read resource id
			pEntry->packageId = pManager->packageCount;
			FRead(&file, (char*)&pEntry->resourceType, sizeof(AssetType)); // read the resource type
			FRead(&file, (char*)&pEntry->resourceSize, sizeof(U64)); // read the resource size
			FRead(&file, (char*)&pEntry->offsetToBlob, sizeof(U64)); // read offset to blob

			// add to list
			if (!pManager->pFirstResourceEntry)
				pManager->pFirstResourceEntry = pEntry;
			else
			{
				ResourceEntry* pLastEntry = pManager->pFirstResourceEntry;
				while (pLastEntry->pNext != nullptr)
				{
					pLastEntry = pLastEntry->pNext;
				}

				pLastEntry->pNext = pEntry;
			}
		}

		// add to list
		PackageEntry* pEntry = (PackageEntry*)AllocFromPool(pManager->pPackageEntryAlloc);
		pEntry->packageFile = file;
		pEntry->pNext = nullptr;

		if (!pManager->pFirstPackageEntry)
			pManager->pFirstPackageEntry = pEntry;
		else
		{
			PackageEntry* pLastEntry = pManager->pFirstPackageEntry;
			while (pLastEntry->pNext != nullptr)
			{
				pLastEntry->pNext = pLastEntry;
			}

			pLastEntry->pNext = pEntry;
		}

		pManager->packageCount++; // increment the package counter

		return true;
	}

	void LoadResource(ResourceManager* pManager, U32 resourceId)
	{
		// loop through the list of loaded assets
		ResourceEntry* pEntry = pManager->pFirstResourceEntry;
		while (pEntry != nullptr)
		{
			if (pEntry->resourceId == resourceId)
			{
				// Load resource from package
				pEntry->pData = AllocFromStack(pManager->pResourceDataAlloc, pEntry->resourceSize);
				ReadFromPackage(pManager, pEntry->packageId, pEntry->offsetToBlob, pEntry->pData, pEntry->resourceSize);
				return;
			}

			pEntry = pEntry->pNext;
		}
	}

	ResourceEntry* GetResourceEntry(ResourceManager* pManager, U32 resourceId)
	{
		ResourceEntry* pEntry = pManager->pFirstResourceEntry;
		while (pEntry != nullptr) // loop through resources
		{
			if (pEntry->resourceId == resourceId)
			{
				// return resource ptr
				return pEntry;
			}

			pEntry = pEntry->pNext;
		}

		return nullptr;
	}

	void UnloadAllResources(ResourceManager* pManager)
	{
		ClearStack(pManager->pResourceDataAlloc);
	}

	void ReadFromPackage(ResourceManager* pManager, U32 packageIndex, U64 offsetToBlob, void* pBuffer, U64 resourceSize)
	{
		PackageEntry* pEntry = pManager->pFirstPackageEntry;
		for (U32 i = 0; i < pManager->packageCount; i++)
		{
			if (i == packageIndex)
			{
				FSeek(&pEntry->packageFile, (I32)offsetToBlob, SEEK_ORIGIN_BEGINING);
				FRead(&pEntry->packageFile, (char*)pBuffer, resourceSize);
			}

			pEntry = pEntry->pNext;
		}
	}

	U32 GetIdWithString(ResourceManager* pManager, const char* pStr)
	{
		StringTableEntry* pEntry = pManager->pFirstStringTableEntry;
		while (pEntry != nullptr)
		{
			if (strcmp(pStr, pEntry->assetName) == 0)
				return pEntry->assetId;

			pEntry = pEntry->pNext;
		}
		return 0;
	}

	ResourceManager CreateResourceManager(const ResourceManagerInfo* info)
	{
		ResourceManager manager = {};
		manager.pPackageEntryAlloc = info->pPackageEntryAllocator;
		manager.pResourceDataAlloc = info->pResourceAllocator;
		manager.pResourceEntryAlloc = info->pEntryAllocator;
		manager.pStringTableAlloc = info->pStringTableEntryAllocator;
		return manager;
	}

	void ShutdownResourceManager(ResourceManager* pManager)
	{
		// Close all package files
		PackageEntry* pEntry = pManager->pFirstPackageEntry;
		while (pEntry != nullptr)
		{
			FClose(&pEntry->packageFile);
			pEntry = pEntry->pNext;
		}

		FreePoolAllocator(pManager->pStringTableAlloc);
		FreePoolAllocator(pManager->pResourceEntryAlloc);
		FreePoolAllocator(pManager->pPackageEntryAlloc);
		FreeStackAllocator(pManager->pResourceDataAlloc);
	}

}