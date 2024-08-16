#include "ResourceManager.h"
#include <File.h>

namespace MarkTech
{
	PoolAllocator* pResourceEntryAlloc = nullptr;
	ResourceEntry* pFirstResourceEntry = nullptr;

	void LoadPackage(const char* pFilepath)
	{
		File file = FOpen(pFilepath, FileAccessType::READ);

		U64 entryCount = 0;
		FRead(&file, (char*)&entryCount, sizeof(U64));

		U64 offsetToEntry = sizeof(U64);
		for (U32 i = 0; i < entryCount; i++)
		{
			ResourceEntry* pEntry = (ResourceEntry*)AllocFromPool(pResourceEntryAlloc);
			pEntry->pNext = nullptr;
			pEntry->pData = nullptr;
			
			FRead(&file, (char*)&pEntry->resourceId, sizeof(U32));

			FRead(&file, (char*)&pEntry->resourceSize, sizeof(U64)); // read the resource size
			
			offsetToEntry += 18;
			pEntry->offsetToEntryInFile = offsetToEntry;
			FSeek(&file, sizeof(U64), SeekOrigin::CURRENT); // skip offsetToBlob

			// add to list
			if (!pFirstResourceEntry)
				pFirstResourceEntry = pEntry;
			else
			{
				ResourceEntry* pLastEntry = pFirstResourceEntry;
				while (pLastEntry->pNext != nullptr)
				{
					pLastEntry->pNext = pLastEntry;
				}

				pLastEntry->pNext = pEntry;
			}
		}

		FClose(&file);
	}

	void LoadResource(U32 resourceId)
	{
		
	}

	void InitResourceManager(U64 sizeInBytes, PoolAllocator* pEntryAllocator)
	{
		pResourceEntryAlloc = pEntryAllocator;
	}

	void ShutdownResourceManager()
	{
		FreePoolAllocator(pResourceEntryAlloc);
	}

}