#include "String\DebugStringManager.h"

namespace MarkTech
{
	DebugStringManager* DebugStringManager::m_gpStringAllocator = nullptr;

	DebugStringManager::DebugStringManager()
		:m_StringAlloc(nullptr, 0)
	{
		if (!m_gpStringAllocator)
			m_gpStringAllocator = this;
	}

	DebugStringManager::~DebugStringManager()
	{
	}

	void DebugStringManager::Init(void* ptr, U64 memorySize)
	{
		m_StringAlloc = StackAllocator(ptr, memorySize);
	}

	void DebugStringManager::Shutdown()
	{
		m_StringAlloc.Clear();
	}

	void* DebugStringManager::AllocMem(U64 sizeInBytes)
	{
		return m_StringAlloc.Alloc(sizeInBytes);
	}

	void* DebugStringManager::AllocStringDbg(U64 sizeInBytes)
	{
		return m_gpStringAllocator->AllocMem(sizeInBytes + 1);
	}
}