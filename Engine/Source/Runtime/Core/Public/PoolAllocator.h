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

	PoolAllocator CreatePoolAllocator(U64 blockSize, U64 blockCount);
	void* AllocFromPool(PoolAllocator* pAllocator);
	void FreeToPool(PoolAllocator* pAllocator, void* ptr);
	void FreePoolAllocator(PoolAllocator* pAllocator);
}