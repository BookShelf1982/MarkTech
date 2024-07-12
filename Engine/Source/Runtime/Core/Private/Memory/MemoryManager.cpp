#include "Memory\MemoryManager.h"
#include "Memory\AlignedAllocator.h"
#include "Debug\Assert.h"

namespace MarkTech
{
	MemoryManager* MemoryManager::m_gpMemManager = nullptr;

	MemoryManager::MemoryManager()
		:m_pMemory(nullptr), m_pMemCursor(nullptr), m_AllocatedMemory(0), m_UsedMemory(0)
	{
		if (!m_gpMemManager)
			m_gpMemManager = this;
	}

	MemoryManager::~MemoryManager()
	{
	}

	void MemoryManager::Init(U64 sizeInBytes)
	{
		m_pMemory = (U8*)AllocAligned((size_t)sizeInBytes, 1);
		m_pMemCursor = m_pMemory;
		m_AllocatedMemory = sizeInBytes;
	}

	void MemoryManager::Shutdown()
	{
		FreeAligned(m_pMemory);
	}

	void* MemoryManager::AllocMem(U64 sizeInBytes, U64 alignment)
	{
		U64 memoryUsedAfter = (m_UsedMemory + sizeInBytes + alignment);
		if (memoryUsedAfter > m_AllocatedMemory)
		{
			MT_ASSERT(memoryUsedAfter < m_AllocatedMemory);
			return nullptr;
		}

		m_pMemCursor += (sizeInBytes + alignment);
		U8* pCursor = m_pMemCursor;
		AlignPointer(pCursor, alignment);
		m_UsedMemory += sizeInBytes;
		return pCursor;
	}

	MemoryQuery MemoryManager::GetMemoryStats()
	{
		MemoryQuery stats = {};
		stats.allocatedMemory = m_gpMemManager->m_AllocatedMemory;
		stats.usedMemory = m_gpMemManager->m_UsedMemory;

		return stats;
	}

	void* MemoryManager::Alloc(U64 sizeInBytes, U64 alignment)
	{
		MT_ASSERT(m_gpMemManager);
		return m_gpMemManager->AllocMem(sizeInBytes, alignment);
	}
}