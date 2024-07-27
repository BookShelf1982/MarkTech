#pragma once
#include <PrimitiveTypes.h>

namespace MarkTech
{
	struct MemoryBlock
	{
		char* pMemoryBlock;
		MemoryBlock* pNext;
	};

	struct PoolAllocator
	{
		char* pMemory;
		MemoryBlock* pBlocks;
		MemoryBlock* pFreeBlocks;
		U64 memorySize;
		U64 blockSize;
	};

	PoolAllocator CreatePoolAllocator(U64 blockSize, U64 blockCount);
	void* AllocFromPool(PoolAllocator* pAllocator);
	void FreeToPool(PoolAllocator* pAllocator, void* ptr);
}