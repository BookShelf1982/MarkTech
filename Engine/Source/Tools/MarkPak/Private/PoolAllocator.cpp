#include "PoolAllocator.h"
#include <stdlib.h>

namespace MarkTech
{
    PoolAllocator CreatePoolAllocator(U64 blockSize, U64 blockCount)
    {
        //MT_ASSERT(blockSize >= sizeof(void*));

        PoolAllocator pool = {};
        pool.blockSize = blockSize;
        pool.memorySize = blockSize * blockCount;
        pool.pMemory = (char*)malloc(pool.memorySize);
        pool.pFreeBlocks = (U64*)pool.pMemory;

        U64* pElement = (U64*)pool.pMemory;
        char* pNextPtr = pool.pMemory;
        for (U64 i = 0; i < blockCount; i++)
        {
            pNextPtr += blockSize;
            
            if (i == blockCount - 1)
                *pElement = (U64)0;
            else
                *pElement = (U64)pNextPtr;

            pElement = (U64*)pNextPtr;
        }

        return pool;
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