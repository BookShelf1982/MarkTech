#include "MemoryPool.h"

CMemoryPool::CMemoryPool(
	const size_t& initialMemoryPoolSize, 
	const size_t& memoryChunkSize, 
	const size_t& minimalSizeToAllocate, 
	bool setMemoryData
)
{
	m_pFirstChunk = NULL;
	m_pLastChunk = NULL;
	m_pCursorChunk = NULL;

	m_nTotalMemoryPoolSize = 0;
	m_nFreeMemoryPoolSize = 0;
	m_nUsedMemoryPoolSize = 0;
	
	m_nMemoryChunkSize = memoryChunkSize;
	m_nMemoryChunkCount = 0;
	m_nObjectCount = 0;

	m_bSetMemoryData = setMemoryData;
	m_nMinimalMemorySizeToAllocate = minimalSizeToAllocate;

	AllocateMemory(initialMemoryPoolSize);
}

CMemoryPool::~CMemoryPool()
{
	FreeAllAllocatedMemory();
	DeallocateAllChunks();
}

void* CMemoryPool::GetMemory(const size_t& memorySize)
{
	size_t nBestMemBlockSize = CalculateBestMemoryBlockSize(memorySize);
	MMemoryChunk* pChunk = nullptr;
	while (!pChunk)
	{
		pChunk = FindSuitableChunkToHoldMemory(nBestMemBlockSize);
		if (!pChunk)
		{
			nBestMemBlockSize = MaxValue(nBestMemBlockSize, CalculateBestMemoryBlockSize(m_nMinimalMemorySizeToAllocate));
			AllocateMemory(nBestMemBlockSize);
		}
	}

	m_nUsedMemoryPoolSize += nBestMemBlockSize;
	m_nFreeMemoryPoolSize -= nBestMemBlockSize;
	m_nObjectCount++;
	SetMemoryChunkValues(pChunk, nBestMemBlockSize);

	return (void*)pChunk->pData;
}

void CMemoryPool::FreeMemory(void* ptr, const size_t& memorySize)
{
	MMemoryChunk* pChunk = FindChunkHoldingPointerTo(ptr);
	if (pChunk)
	{
		FreeChunks(pChunk);
	}

	m_nObjectCount--;
}

bool CMemoryPool::AllocateMemory(const size_t& memorySize)
{
	uint32_t nNeededChunks = CalculateNeededCunks(memorySize);
	size_t nBestMemorySize = CalculateBestMemoryBlockSize(memorySize);

	unsigned char* pNewMemBlock = (unsigned char*)malloc(nBestMemorySize);
	MMemoryChunk* pNewChunks = (MMemoryChunk*)malloc(nNeededChunks * sizeof(MMemoryChunk));

	m_nTotalMemoryPoolSize += nBestMemorySize;
	m_nFreeMemoryPoolSize += nBestMemorySize;
	m_nMemoryChunkCount += nNeededChunks;

	if (m_bSetMemoryData)
		memset((void*)pNewMemBlock, 0, nBestMemorySize);

	return LinkChunksToData(pNewChunks, nNeededChunks, pNewMemBlock);
}

void CMemoryPool::FreeAllAllocatedMemory()
{
	MMemoryChunk* pChunk = m_pFirstChunk;
	while (pChunk)
	{
		if (pChunk->bIsAllocationChunk)
		{
			free((void*)pChunk->pData);
		}
		pChunk = pChunk->pNext;
	}
}

uint32_t CMemoryPool::CalculateNeededCunks(const size_t& memorySize)
{
	float f = ((float)memorySize / (float)m_nMemoryChunkSize);
	return ((uint32_t)ceil(f));
}

size_t CMemoryPool::CalculateBestMemoryBlockSize(const size_t& requestedMemorySize)
{
	uint32_t nNeededChunks = CalculateNeededCunks(requestedMemorySize);
	return ((nNeededChunks * m_nMemoryChunkSize));
}

MMemoryChunk* CMemoryPool::FindSuitableChunkToHoldMemory(const size_t& memorySize)
{
	uint32_t nChunksToSkip = 0;
	bool bContinueSearch = true;
	MMemoryChunk* pChunk = m_pCursorChunk;

	for (uint32_t i = 0; i < m_nMemoryChunkCount; i++)
	{
		if (pChunk)
		{
			if (pChunk == m_pLastChunk)
			{
				pChunk = m_pFirstChunk;
			}

			if (pChunk->nDataSize >= memorySize)
			{
				if (pChunk->nUsedSize == 0)
				{
					m_pCursorChunk = pChunk;
					return pChunk;
				}
			}

			nChunksToSkip = CalculateNeededCunks(pChunk->nUsedSize);
			if (nChunksToSkip == 0) nChunksToSkip = 1;

			pChunk = SkipChunks(pChunk, nChunksToSkip);
		}
		else
		{
			bContinueSearch = false;
		}
	}

	return nullptr;
}

MMemoryChunk* CMemoryPool::FindChunkHoldingPointerTo(void* memoryBlock)
{
	MMemoryChunk* pTempChunk = m_pFirstChunk;

	while (pTempChunk)
	{
		if (pTempChunk->pData == (unsigned char*)memoryBlock)
		{
			break;
		}
		pTempChunk = pTempChunk->pNext;
	}
	return pTempChunk;
}

MMemoryChunk* CMemoryPool::SkipChunks(MMemoryChunk* startChunk, size_t chunksToSkip)
{
	MMemoryChunk* pChunk = startChunk;
	for (uint32_t i = 0; i < chunksToSkip; i++)
	{
		if (pChunk)
		{
			pChunk = pChunk->pNext;
		}
		else
		{
			break;
		}
	}

	return pChunk;
}

MMemoryChunk* CMemoryPool::SetChunkDefaults(MMemoryChunk* chunk)
{
	if (chunk)
	{
		chunk->pData = nullptr;
		chunk->nDataSize = 0;
		chunk->nUsedSize = 0;
		chunk->bIsAllocationChunk = false;
		chunk->nDebugIndex = 0;
		chunk->pNext = nullptr;
	}
	return chunk;
}

void CMemoryPool::FreeChunks(MMemoryChunk* chunk)
{
	MMemoryChunk* pChunk = chunk;
	uint32_t nChunkCount = CalculateNeededCunks(pChunk->nUsedSize);

	for (uint32_t i = 0; i < nChunkCount; i++)
	{
		if (pChunk)
		{
			if (m_bSetMemoryData)
		  	{
		        memset(((void*)pChunk->pData), 0, m_nMemoryChunkSize);
		  	}

			pChunk->nUsedSize = 0;
			m_nUsedMemoryPoolSize -= m_nMemoryChunkSize;
			pChunk = pChunk->pNext;
		}
	}
}

void CMemoryPool::DeallocateAllChunks()
{
	MMemoryChunk* pChunk = m_pFirstChunk;

	while(pChunk)
	{
		if(pChunk->bIsAllocationChunk)
		{	
			free(((void*)pChunk));
			break;
		}
		else
		{
			pChunk = pChunk->pNext ;
		}
	}
}

bool CMemoryPool::LinkChunksToData(MMemoryChunk* chunks, uint32_t chunkCount, unsigned char* newMemoryBlock)
{
	MMemoryChunk* pChunk = NULL;
	uint32_t nMemOffset = 0;
	bool bAllocationChunkAssigned = false;

	for (uint32_t i = 0; i < chunkCount; i++)
	{
		if (!m_pFirstChunk)
		{
			m_pFirstChunk = SetChunkDefaults(&chunks[0]);
			m_pCursorChunk = m_pFirstChunk;
			m_pLastChunk = m_pFirstChunk;
		}
		else
		{
			pChunk = SetChunkDefaults(&chunks[i]);
			m_pLastChunk->pNext = pChunk;
			m_pLastChunk = pChunk;
		}
		
		nMemOffset = (i * (uint32_t)m_nMemoryChunkSize);
		m_pLastChunk->pData = &newMemoryBlock[nMemOffset];
		m_pLastChunk->nDebugIndex = i;

		if (!bAllocationChunkAssigned)
		{
			m_pLastChunk->bIsAllocationChunk = true;
			bAllocationChunkAssigned = true;
		}
	}

	return RecalcChunkMemorySize(m_pFirstChunk, m_nMemoryChunkCount);
}

void CMemoryPool::SetMemoryChunkValues(MMemoryChunk* chunk, const size_t& memBlockSize)
{
	if (chunk)
	{
		chunk->nUsedSize = memBlockSize;
	}
}

bool CMemoryPool::RecalcChunkMemorySize(MMemoryChunk* chunk, uint32_t chunkCount)
{
	uint32_t nMemOffset = 0;
	for (uint32_t i = 0; i < chunkCount; i++)
	{
		if (chunk)
		{
			nMemOffset = (i * (uint32_t)m_nMemoryChunkSize);
			chunk->nDataSize = (uint32_t)m_nTotalMemoryPoolSize - nMemOffset;
			chunk = chunk->pNext;
		}
		else
		{
			break;
			return false;
		}
	}
	return true;
}

size_t CMemoryPool::MaxValue(const size_t& a, const size_t& b) const
{
	if(a < b)
		return a;

	return b;
}

bool CMemoryPool::IsValidPointer(void* ptr)
{
	MMemoryChunk* pChunk = m_pFirstChunk;
	while (pChunk)
	{
		if (pChunk->pData == (unsigned char*)ptr)
		{
			return true;
		}
		pChunk = pChunk->pNext;
	}
	return false;
}
