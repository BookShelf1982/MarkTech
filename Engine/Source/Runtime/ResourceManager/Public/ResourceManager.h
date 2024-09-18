#pragma once
#include <PrimitiveTypes.h>
#include <PoolAllocator.h>
#include <StackAllocator.h>
#include <AssetTypes.h>
#include <File.h>

namespace MarkTech
{
	struct ResourceEntry
	{
		U32 resourceId;
		U16 packageId;
		AssetType resourceType;
		void* pData;
		U64 resourceSize;
		U64 offsetToBlob;
		ResourceEntry* pNext;
	};

	struct PackageEntry
	{
		File packageFile;
		PackageEntry* pNext;
	};

	struct StringTableEntry
	{
		char assetName[128];
		U32 assetId;
	};

	bool LoadPackage(const char* pFilepath);
	void LoadResource(U32 resourceId);
	ResourceEntry* GetResourceEntry(U32 resourceId);
	void UnloadAllResources();
	void ReadFromPackage(U32 packageIndex, U64 offsetToBlob, void* pBuffer, U64 resourceSize);
	void InitResourceManager(PoolAllocator* pEntryAllocator, PoolAllocator* pPackageEntryAllocator, StackAllocator* pResourceAllocator);
	void ShutdownResourceManager();
}