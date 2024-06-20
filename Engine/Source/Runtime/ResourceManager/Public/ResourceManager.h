#pragma once
#include <PrimitiveTypes.h>
#include <DSA\Dictionary.h>
#include <Memory\StackAllocator.h>

namespace MarkTech
{
	typedef U64 ResourceHandle;

	class ResourceManager
	{
	public:
		ResourceManager();
		~ResourceManager();

		void Init(U64 sizeInBytes);
		void Shutdown();

		static void LoadAssetPackage(const char* filepath);
	private:
		Dictionary<ResourceHandle, U8*> m_LookupList;
		StackAllocator m_AssetAllocator;

		static ResourceManager* m_gpResourceManager;
	};
}