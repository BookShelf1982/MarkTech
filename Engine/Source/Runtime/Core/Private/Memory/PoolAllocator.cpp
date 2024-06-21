#include "Memory\PoolAllocator.h"
#include "Memory\MemoryManager.h"
#include <memory.h>

namespace MarkTech
{
	PoolAllocator::PoolAllocator()
		:m_pMemory(nullptr), m_pFreeElements(nullptr), m_pLinkStart(nullptr), m_ElementsSize(0), m_NumOfElements(0)
	{
	}

	PoolAllocator::PoolAllocator(U64 sizeInBytes, U64 blockSizeInBytes, U64 alignment)
		:m_pMemory(nullptr), m_pFreeElements(nullptr), m_pLinkStart(nullptr), m_ElementsSize(blockSizeInBytes), m_NumOfElements((U32)(sizeInBytes / blockSizeInBytes))
	{
		if ((sizeInBytes % blockSizeInBytes) == 0)
		{
			m_pLinkStart = (FreeElement*)MemoryManager::Alloc((size_t)(sizeInBytes + (m_NumOfElements * sizeof(FreeElement))), sizeof(void*));
			m_pMemory = reinterpret_cast<U8*>((U8*)(m_pLinkStart) + m_NumOfElements * sizeof(FreeElement));

			U64 offset = 0;
			for (U32 i = 0; i < m_NumOfElements; i++)
			{
				m_pLinkStart[i].pBlock = &m_pMemory[offset];

				m_pLinkStart[i].pNext = &m_pLinkStart[i + 1];

				offset = offset + blockSizeInBytes;
			}

			m_pLinkStart[m_NumOfElements - 1].pNext = nullptr;
			
			m_pFreeElements = m_pLinkStart;
		}
	}

	PoolAllocator::~PoolAllocator()
	{
	}

	void* PoolAllocator::Alloc()
	{
		if (!m_pFreeElements)
			return nullptr;

		// Remove first link from 
		FreeElement* pElement = m_pFreeElements;
		m_pFreeElements = m_pFreeElements->pNext;

		return pElement->pBlock;
	}

	void PoolAllocator::Free(void* ptr)
	{
		// Find link coresponding to the requested pointer
		FreeElement* pElement = nullptr;

		for (U32 i = 0; i < m_NumOfElements; i++)
		{
			if (m_pLinkStart[i].pBlock == ptr)
			{
				pElement = &m_pLinkStart[i];
				break;
			}
		}

		if (!pElement)
			return;

		memset(pElement->pBlock, 0, m_ElementsSize);

		// Add memory block at begining
		pElement->pNext = m_pFreeElements;
		m_pFreeElements = pElement;
	}
}