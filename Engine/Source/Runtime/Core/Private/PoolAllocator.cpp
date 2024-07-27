#include "PoolAllocator.h"
#include "MemoryArena.h"

namespace MarkTech
{
    PoolAllocator CreatePoolAllocator(U64 blockSize, U64 blockCount)
    {
        PoolAllocator pool = {};
        pool.blockSize = blockSize;
        pool.memorySize = blockSize * blockCount;
        pool.pMemory = (char*)AllocFromMemoryArena(pool.memorySize);
        pool.pBlocks = (MemoryBlock*)AllocFromMemoryArena(sizeof(MemoryBlock) * blockCount);
        pool.pFreeBlocks = pool.pBlocks;
        
        MemoryBlock* pBlock = pool.pBlocks;
        char* pMem = pool.pMemory;
        while (pBlock != nullptr)
        {
            pBlock->pMemoryBlock = pMem;
            pBlock = pBlock->pNext;
            pMem += pool.blockSize;
        }

        return pool;
    }
}