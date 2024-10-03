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
		StringTableEntry* pNext;
	};

	struct ResourceManagerInfo
	{
		PoolAllocator* pEntryAllocator;
		PoolAllocator* pPackageEntryAllocator;
		PoolAllocator* pStringTableEntryAllocator;
		StackAllocator* pResourceAllocator;	
	};

	struct ResourceManager
	{
		PoolAllocator* pResourceEntryAlloc;
		PoolAllocator* pPackageEntryAlloc;
		PoolAllocator* pStringTableAlloc;
		StackAllocator* pResourceDataAlloc;
		ResourceEntry* pFirstResourceEntry;
		PackageEntry* pFirstPackageEntry;
		StringTableEntry* pFirstStringTableEntry;
		U16 packageCount;
	};

	bool LoadPackage(ResourceManager* pManager, const char* pFilepath);
	void LoadResource(ResourceManager* pManager, U32 resourceId);
	ResourceEntry* GetResourceEntry(ResourceManager* pManager, U32 resourceId);
	void UnloadAllResources(ResourceManager* pManager);
	void ReadFromPackage(ResourceManager* pManager, U32 packageIndex, U64 offsetToBlob, void* pBuffer, U64 resourceSize);
	U32 GetIdWithString(ResourceManager* pManager, const char* pStr);
	ResourceManager CreateResourceManager(const ResourceManagerInfo* pInfo);
	void ShutdownResourceManager(ResourceManager* pManager);
}