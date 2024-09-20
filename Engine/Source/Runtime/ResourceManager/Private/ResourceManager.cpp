#include "ResourceManager.h"
#include <File.h>

namespace MarkTech
{
	PoolAllocator* gpResourceEntryAlloc = nullptr;
	PoolAllocator* gpPackageEntryAlloc = nullptr;
	PoolAllocator* gpStringTableAlloc = nullptr;
	StackAllocator* gpResourceDataAlloc = nullptr;
	ResourceEntry* gpFirstResourceEntry = nullptr;
	PackageEntry* gpFirstPackageEntry = nullptr;
	StringTableEntry* gpFirstStringTableEntry = nullptr;
	U16 gPackageCount = 0;

	bool LoadPackage(const char* pFilepath)
	{
		File file = FOpen(pFilepath, FileAccessType::READ);
		if (!file.isOpened)
			return false;

		U64 entryCount = 0;
		U32 packageFlags = 0;
		// Skip package signature
		U8 signatureLen = 0;
		FRead(&file, (char*)&signatureLen, 1);
		if (signatureLen != 0)
			FSeek(&file, signatureLen, SeekOrigin::CURRENT);

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
				StringTableEntry* pEntry = (StringTableEntry*)AllocFromPool(gpStringTableAlloc);
				pEntry->pNext = nullptr;

				FRead(&file, (char*)&pEntry->assetId, sizeof(U32));
				U8 strLen = 0;
				FRead(&file, (char*)&strLen, sizeof(U8));
				FRead(&file, (char*)pEntry->assetName, strLen);
				pEntry->assetName[strLen] = 0;

				// add to list
				if (!gpFirstStringTableEntry)
					gpFirstStringTableEntry = pEntry;
				else
				{
					StringTableEntry* pLastEntry = gpFirstStringTableEntry;
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
			ResourceEntry* pEntry = (ResourceEntry*)AllocFromPool(gpResourceEntryAlloc);
			pEntry->pNext = nullptr;
			pEntry->pData = nullptr;
			
			FRead(&file, (char*)&pEntry->resourceId, sizeof(U32)); // read resource id
			pEntry->packageId = gPackageCount;
			FRead(&file, (char*)&pEntry->resourceType, sizeof(AssetType)); // read the resource type
			FRead(&file, (char*)&pEntry->resourceSize, sizeof(U64)); // read the resource size
			FRead(&file, (char*)&pEntry->offsetToBlob, sizeof(U64)); // read offset to blob

			// add to list
			if (!gpFirstResourceEntry)
				gpFirstResourceEntry = pEntry;
			else
			{
				ResourceEntry* pLastEntry = gpFirstResourceEntry;
				while (pLastEntry->pNext != nullptr)
				{
					pLastEntry = pLastEntry->pNext;
				}

				pLastEntry->pNext = pEntry;
			}
		}

		// add to list
		PackageEntry* pEntry = (PackageEntry*)AllocFromPool(gpPackageEntryAlloc);
		pEntry->packageFile = file;
		pEntry->pNext = nullptr;

		if (!gpFirstPackageEntry)
			gpFirstPackageEntry = pEntry;
		else
		{
			PackageEntry* pLastEntry = gpFirstPackageEntry;
			while (pLastEntry->pNext != nullptr)
			{
				pLastEntry->pNext = pLastEntry;
			}

			pLastEntry->pNext = pEntry;
		}

		gPackageCount++; // increment the package counter

		return true;
	}

	void LoadResource(U32 resourceId)
	{
		// loop through the list of loaded assets
		ResourceEntry* pEntry = gpFirstResourceEntry;
		while (pEntry != nullptr)
		{
			if (pEntry->resourceId == resourceId)
			{
				// Load resource from package
				pEntry->pData = AllocFromStack(gpResourceDataAlloc, pEntry->resourceSize);
				ReadFromPackage(pEntry->packageId, pEntry->offsetToBlob, pEntry->pData, pEntry->resourceSize);
				return;
			}

			pEntry = pEntry->pNext;
		}
	}

	ResourceEntry* GetResourceEntry(U32 resourceId)
	{
		ResourceEntry* pEntry = gpFirstResourceEntry;
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

	void UnloadAllResources()
	{
		ClearStack(gpResourceDataAlloc);
	}

	void ReadFromPackage(U32 packageIndex, U64 offsetToBlob, void* pBuffer, U64 resourceSize)
	{
		PackageEntry* pEntry = gpFirstPackageEntry;
		for (U32 i = 0; i < gPackageCount; i++)
		{
			if (i == packageIndex)
			{
				FSeek(&pEntry->packageFile, (I32)offsetToBlob, SeekOrigin::BEGINING);
				FRead(&pEntry->packageFile, (char*)pBuffer, resourceSize);
			}

			pEntry = pEntry->pNext;
		}
	}

	U32 GetIdWithString(const char* pStr)
	{
		StringTableEntry* pEntry = gpFirstStringTableEntry;
		while (pEntry != nullptr)
		{
			if (strcmp(pStr, pEntry->assetName) == 0)
				return pEntry->assetId;

			pEntry = pEntry->pNext;
		}
		return 0;
	}

	void InitResourceManager(
		PoolAllocator* pEntryAllocator, 
		PoolAllocator* pPackageEntryAllocator, 
		PoolAllocator* pStringTableEntryAllocator, 
		StackAllocator* pResourceAllocator
	)
	{
		gpResourceEntryAlloc = pEntryAllocator;
		gpPackageEntryAlloc = pPackageEntryAllocator;
		gpResourceDataAlloc = pResourceAllocator;
		gpStringTableAlloc = pStringTableEntryAllocator;
	}

	void ShutdownResourceManager()
	{
		// Close all package files
		PackageEntry* pEntry = gpFirstPackageEntry;
		while (pEntry != nullptr)
		{
			FClose(&pEntry->packageFile);
			pEntry = pEntry->pNext;
		}

		FreePoolAllocator(gpStringTableAlloc);
		FreePoolAllocator(gpResourceEntryAlloc);
		FreePoolAllocator(gpPackageEntryAlloc);
		FreeStackAllocator(gpResourceDataAlloc);
	}

}