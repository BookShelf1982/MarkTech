#include "Memory\StackAllocator.h"
#include "Memory\AlignedAllocator.h"
#include <stdlib.h>
#include <string.h>

namespace MarkTech
{
	StackAllocator::StackAllocator(U32 sizeInBytes, U32 alignment)
		:m_pAllocatedBytes(nullptr), m_pTopMarker(nullptr), m_pRollbackMarker(nullptr), m_AllocatedSize(sizeInBytes), m_UsedSize(0)
	{
		m_pAllocatedBytes = (U8*)AllocAligned(sizeInBytes, alignment);
		m_pTopMarker = m_pAllocatedBytes;
	}

	StackAllocator::~StackAllocator()
	{
		FreeAligned(m_pAllocatedBytes);
	}

	void* StackAllocator::Alloc(U32 sizeInBytes)
	{
		if (m_AllocatedSize < m_UsedSize)
			return nullptr;

		void* ptr = m_pTopMarker;
		m_pTopMarker = m_pTopMarker + sizeInBytes;
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
		U32 size = pMarker - m_pAllocatedBytes;
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
		m_pRollbackMarker = m_pTopMarker;
		m_UsedSize = 0;
	}
}