#include "PoolAllocator.h"
#include "Assert.h"
#include <stdlib.h>

namespace MarkTech
{
	void CreatePoolAllocator(PoolAllocator* pPool, U64 blockSize, U64 blockCount)
	{
		MT_ASSERT(blockSize >= sizeof(void*));

		pPool->blockSize = blockSize;
		pPool->memorySize = blockSize * blockCount;
		pPool->pMemory = (char*)malloc(pPool->memorySize);
		pPool->pFreeBlocks = (U64*)pPool->pMemory;

		U64* pElement = (U64*)pPool->pMemory;
		char* pNextPtr = pPool->pMemory;
		for (U64 i = 0; i < blockCount; i++)
		{
			pNextPtr += blockSize;

			if (i == blockCount - 1)
				*pElement = (U64)0;
			else
				*pElement = (U64)pNextPtr;

			pElement = (U64*)pNextPtr;
		}
	}

	void* AllocFromPool(PoolAllocator* pAllocator)
	{
		void* pBlock = pAllocator->pFreeBlocks;
		pAllocator->pFreeBlocks = (U64*)*pAllocator->pFreeBlocks;
		return pBlock;
	}

	void FreeToPool(PoolAllocator* pAllocator, void* ptr)
	{
		// Overwrite the pointer to be a next pointer
		U64* ptr64 = (U64*)ptr;
		*ptr64 = 0;

		// Get the next pointer pointing to 0 
		U64* pNextPointer = pAllocator->pFreeBlocks;
		while (*pNextPointer != 0)
		{
			pNextPointer = (U64*)*pNextPointer;
		}

		// Reassign this pointrt to the freed pointer
		*pNextPointer = (U64)ptr;
	}

	void FreePoolAllocator(PoolAllocator* pAllocator)
	{
		free(pAllocator->pMemory);
	}
}