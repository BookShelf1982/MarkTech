#include "Memory\StackAllocator.h"
#include "Memory\MemoryManager.h"
#include <string.h>

namespace MarkTech
{
	StackAllocator::StackAllocator(U64 sizeInBytes, U64 alignment)
		:m_pAllocatedBytes(nullptr), m_pTopMarker(nullptr), m_AllocatedSize(sizeInBytes), m_UsedSize(0)
	{
		m_pAllocatedBytes = (U8*)MemoryManager::Alloc(sizeInBytes, alignment);
		m_pTopMarker = m_pAllocatedBytes;
	}

	StackAllocator::StackAllocator(void* memory, U64 sizeInBytes)
		:m_pAllocatedBytes((U8*)memory), m_pTopMarker((U8*)memory), m_AllocatedSize(sizeInBytes), m_UsedSize(0)
	{
	}

	StackAllocator::~StackAllocator()
	{
	}

	void* StackAllocator::Alloc(U64 sizeInBytes)
	{
		if ((m_UsedSize + sizeInBytes) > m_AllocatedSize)
			return nullptr;

		void* ptr = m_pTopMarker;
		m_pTopMarker += sizeInBytes;
		m_UsedSize += sizeInBytes;
		return ptr;
	}

	StackMarker StackAllocator::Mark()
	{
		return m_pTopMarker - m_pAllocatedBytes;
	}

	void StackAllocator::FreeToMark(StackMarker marker)
	{
		U8* pMarker = m_pAllocatedBytes + marker;
		U64 size = pMarker - m_pAllocatedBytes;
		if (pMarker >= m_pTopMarker)
			return;

		memset(pMarker, 0, m_AllocatedSize - size);
		m_pTopMarker = pMarker;
		m_UsedSize -= (m_pTopMarker - (m_pAllocatedBytes + marker));
	}

	void StackAllocator::Clear()
	{
		memset(m_pAllocatedBytes, 0, m_AllocatedSize);
		m_pTopMarker = m_pAllocatedBytes;
		m_UsedSize = 0;
	}
}