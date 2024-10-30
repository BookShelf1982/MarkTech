#include "PoolAllocator.h"
#include "Assert.h"
#include <stdlib.h>

namespace MarkTech
{
	void CreatePoolAllocator(PoolAllocator& allocator, U64 blockSize, U64 blockCount)
	{
		MT_ASSERT(blockSize >= sizeof(void*));

		allocator.blockSize = blockSize;
		allocator.memorySize = blockSize * blockCount;
		allocator.pMemory = (char*)malloc(allocator.memorySize);
		allocator.pFreeBlocks = (U64*)allocator.pMemory;

		U64* pElement = (U64*)allocator.pMemory;
		char* pNextPtr = allocator.pMemory;
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

	void* AllocFromPool(PoolAllocator& allocator)
	{
		void* pBlock = allocator.pFreeBlocks;
		allocator.pFreeBlocks = (U64*)*allocator.pFreeBlocks;
		return pBlock;
	}

	void FreeToPool(PoolAllocator& allocator, void* ptr)
	{
		// Overwrite the pointer to be a next pointer
		U64* ptr64 = (U64*)ptr;
		*ptr64 = 0;

		// Get the next pointer pointing to 0 
		U64* pNextPointer = allocator.pFreeBlocks;
		while (*pNextPointer != 0)
		{
			pNextPointer = (U64*)*pNextPointer;
		}

		// Reassign this pointrt to the freed pointer
		*pNextPointer = (U64)ptr;
	}

	void FreePoolAllocator(PoolAllocator& allocator)
	{
		free(allocator.pMemory);
	}
}