#pragma once
#include <PrimitiveTypes.h>

namespace MarkTech
{
	struct PoolAllocator
	{
		char* pMemory;
		U64* pFreeBlocks;
		U64 memorySize;
		U64 blockSize;
	};

	void CreatePoolAllocator(PoolAllocator& allocator, U64 blockSize, U64 blockCount);
	void* AllocFromPool(PoolAllocator& allocator);
	void FreeToPool(PoolAllocator& allocator, void* ptr);
	void FreePoolAllocator(PoolAllocator& allocator);
}