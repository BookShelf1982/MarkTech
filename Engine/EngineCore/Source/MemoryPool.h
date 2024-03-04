#pragma once
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

struct MMemoryChunk
{
	unsigned char* pData;
	size_t nDataSize;
	size_t nUsedSize;
	bool bIsAllocationChunk;
	uint32_t nDebugIndex;

	MMemoryChunk* pNext;
};

class CMemoryPool
{
public:
	CMemoryPool(const size_t& initialMemoryPoolSize,
				const size_t& memoryChunkSize,
				const size_t& minimalSizeToAllocate,
				bool setMemoryData);

	~CMemoryPool();

	void* GetMemory(const size_t& memorySize);
	void FreeMemory(void* ptr, const size_t& memorySize);
	bool IsValidPointer(void* ptr);

private:

	bool AllocateMemory(const size_t& memorySize);
	void FreeAllAllocatedMemory();

	uint32_t CalculateNeededCunks(const size_t& memorySize);
	size_t CalculateBestMemoryBlockSize(const size_t& requestedMemorySize);

	MMemoryChunk* FindSuitableChunkToHoldMemory(const size_t& memorySize);
	MMemoryChunk* FindChunkHoldingPointerTo(void* memoryBlock);
	MMemoryChunk* SkipChunks(MMemoryChunk* startChunk, size_t chunksToSkip);
	MMemoryChunk* SetChunkDefaults(MMemoryChunk* chunk);

	void FreeChunks(MMemoryChunk* chunk);
	void DeallocateAllChunks();

	bool LinkChunksToData(MMemoryChunk* chunks, uint32_t chunkCount, unsigned char* newMemoryBlock);
	void SetMemoryChunkValues(MMemoryChunk* chunk, const size_t& memBlockSize);
	bool RecalcChunkMemorySize(MMemoryChunk* chunk, uint32_t chunkCount);
	size_t MaxValue(const size_t& a, const size_t& b) const;

	MMemoryChunk* m_pFirstChunk;
	MMemoryChunk* m_pLastChunk;
	MMemoryChunk* m_pCursorChunk;

	size_t m_nTotalMemoryPoolSize;
	size_t m_nUsedMemoryPoolSize;
	size_t m_nFreeMemoryPoolSize;

	size_t m_nMemoryChunkSize;

	uint32_t m_nMemoryChunkCount;
	uint32_t m_nObjectCount;

	bool m_bSetMemoryData;

	size_t m_nMinimalMemorySizeToAllocate;
};
