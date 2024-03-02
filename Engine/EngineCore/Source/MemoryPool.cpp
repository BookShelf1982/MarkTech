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
	m_nTotalMemoryPoolSize = 0;
	m_nUsedMemoryPoolSize = 0;
	
	m_nMemoryChunkSize = memoryChunkSize;
	m_nMemoryChunkCount = 0;
	m_nObjectCount = 0;

	m_bSetMemoryData = setMemoryData;
	m_nMinimalMemorySizeToAllocate = minimalSizeToAllocate;
}

CMemoryPool::~CMemoryPool()
{
}
