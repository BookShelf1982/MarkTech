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
#ifdef MT_CORELIB
	PoolAllocator CreatePoolAllocator(U64 blockSize, U64 blockCount);
	void* AllocFromPool(PoolAllocator* pAllocator);
	void FreeToPool(PoolAllocator* pAllocator, void* ptr);
	void FreePoolAllocator(PoolAllocator* pAllocator);
}
#else
}

typedef MarkTech::PoolAllocator (*PFN_CreatePoolAllocator)(MarkTech::U64 blockSize, MarkTech::U64 blockCount);
typedef void (*PFN_FreePoolAllocator)(MarkTech::PoolAllocator* pAllocator);
typedef void* (*PFN_AllocFromPool)(MarkTech::PoolAllocator* pAllocator);
typedef void (*PFN_FreeToPool)(MarkTech::PoolAllocator* pAllocator, void* ptr);

extern PFN_CreatePoolAllocator CreatePoolAllocator;
extern PFN_FreePoolAllocator FreePoolAllocator;
extern PFN_AllocFromPool AllocFromPool;
extern PFN_FreeToPool FreeToPool;
#endif