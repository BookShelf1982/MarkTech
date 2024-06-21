#pragma once
#include <PrimitiveTypes.h>
#include <DSA\Dictionary.h>
#include <Memory\StackAllocator.h>

namespace MarkTech
{
	typedef U64 ResourceHandle;

	struct PackageMetadata
	{
		U64 blobSizeInBytes;
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

	struct LookupData
	{
		EntryType type;
		U8* pData;
		U64 size;
	};

	class ResourceManager
	{
	public:
		ResourceManager();
		~ResourceManager();

		void Init(U64 sizeInBytes);
		void Shutdown();

		void LoadPackage(const char* filepath);
		bool LoadResourceWithId(U64 id);
		bool LoadResourceFromPackage(U64 resource, U8 package);

		static void LoadResourcePackage(const char* filepath);
		static bool LoadResource(U64 id);
	private:
		Dictionary<ResourceHandle, LookupData> m_LookupList;
		Dictionary<ResourceHandle, U8> m_ResourcePackageOwnership;
		Dictionary<U32, char*> m_PackageFilename;
		U32 m_PackageCount;
		StackAllocator m_AssetAllocator;

		static ResourceManager* m_gpResourceManager;
	};
}