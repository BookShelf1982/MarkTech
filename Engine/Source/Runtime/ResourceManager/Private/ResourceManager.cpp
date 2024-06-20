#include "ResourceManager.h"
#include <Memory\MemoryManager.h>

namespace MarkTech
{
	ResourceManager* ResourceManager::m_gpResourceManager = nullptr;

	ResourceManager::ResourceManager()
		:m_AssetAllocator(nullptr, 0)
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
		m_AssetAllocator.Clear();
	}

	void ResourceManager::LoadAssetPackage(const char* filepath)
	{
	}
}