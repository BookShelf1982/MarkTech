#pragma once
#include <PrimitiveTypes.h>
#include <PoolAllocator.h>
#include <AssetTypes.h>

namespace MarkTech
{
	struct ResourceEntry
	{
		U32 resourceId;
		void* pData;
		U64 resourceSize;
		U64 offsetToEntryInFile;
		ResourceEntry* pNext;
	};

	void LoadPackage(const char* pFilepath);
	void LoadResource(U32 resourceId);
	void InitResourceManager(U64 sizeInBytes, PoolAllocator* pEntryAllocator);
	void ShutdownResourceManager();
}