#include "Memory\StackAllocator.h"
#include <Memory\AlignedAllocator.h>
#include <stdlib.h>
#include <string.h>

namespace MarkTech
{
	StackAllocator::StackAllocator(U32 sizeInBytes, U32 alignment)
		:m_pAllocatedBytes(nullptr), m_pTopMarker(nullptr), m_pRollbackMarker(nullptr), m_AllocatedSize(sizeInBytes)
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
		void* ptr = m_pTopMarker;
		m_pTopMarker = m_pTopMarker + sizeInBytes;
		return ptr;
	}

	void StackAllocator::Mark()
	{
		m_pRollbackMarker = m_pTopMarker;
	}

	void StackAllocator::FreeToMark()
	{
		if (m_pRollbackMarker == m_pTopMarker)
			return;

		memset(m_pRollbackMarker, 0,  m_pTopMarker - m_pRollbackMarker);
		m_pTopMarker = m_pRollbackMarker;
	}

	void StackAllocator::Clear()
	{
		memset(m_pAllocatedBytes, 0, m_AllocatedSize);
		m_pTopMarker = m_pAllocatedBytes;
		m_pRollbackMarker = m_pTopMarker;
	}
}