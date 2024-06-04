#include "Memory\MemoryManager.h"
#include "Memory\AlignedAllocator.h"

namespace MarkTech
{
	MemoryManager* MemoryManager::m_gpMemManager = nullptr;

	MemoryManager::MemoryManager()
		:m_pMemory(nullptr), m_pMemCursor(nullptr), m_AllocatedMemory(0), m_AllocatedDebugMemory(0), m_UsedMemory(0), m_UsedDebugMemory(0), m_pDbgMemCursor(0)
	{
		if (!m_gpMemManager)
			m_gpMemManager = this;
	}

	MemoryManager::~MemoryManager()
	{
	}

	void MemoryManager::Init(U64 sizeInBytes, U64 debugSizeInBytes)
	{
		m_pMemory = (U8*)AllocAligned((size_t)sizeInBytes + debugSizeInBytes, 1);
		m_pMemCursor = m_pMemory;
		m_AllocatedMemory = sizeInBytes;
		m_AllocatedDebugMemory = debugSizeInBytes;
		m_pDbgMemCursor = m_pMemCursor + debugSizeInBytes;
	}

	void MemoryManager::Shutdown()
	{
		FreeAligned(m_pMemory);
	}

	void* MemoryManager::AllocMem(U64 sizeInBytes, U64 alignment)
	{
		if ((m_UsedMemory + sizeInBytes + alignment) > m_AllocatedMemory)
			return nullptr;

		m_pMemCursor += (sizeInBytes + alignment);
		U8* pCursor = m_pMemCursor;
		AlignPointer(pCursor, alignment);
		return pCursor;
	}

	void* MemoryManager::AllocMemDbg(U64 sizeInBytes, U64 alignment)
	{
		if ((m_UsedDebugMemory + sizeInBytes + alignment) > m_AllocatedDebugMemory)
			return nullptr;

		m_pDbgMemCursor += (sizeInBytes + alignment);
		U8* pCursor = m_pDbgMemCursor;
		AlignPointer(pCursor, alignment);
		return pCursor;
	}

	void* MemoryManager::AllocDbg(U64 sizeInBytes, U64 alignment)
	{
		return m_gpMemManager->AllocMemDbg(sizeInBytes, alignment);
	}

	void* MemoryManager::Alloc(U64 sizeInBytes, U64 alignment)
	{
		return m_gpMemManager->AllocMem(sizeInBytes, alignment);
	}
}