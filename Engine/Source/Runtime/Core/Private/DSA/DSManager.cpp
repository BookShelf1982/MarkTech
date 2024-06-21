#include "DSA\DSManager.h"
#include "Memory\MemoryManager.h"

namespace MarkTech
{
    DSManager* DSManager::m_gpDSManager = nullptr;

    DSManager::DSManager()
        :m_AllocatorCount(0), m_pPoolAllocs(nullptr)
    {
        if (!m_gpDSManager)
            m_gpDSManager = this;
    }

    DSManager::~DSManager()
    {
    }

    void DSManager::Init(U32 blockCount, U32 allocatorCount)
    {
        m_AllocatorCount = allocatorCount;
        m_pPoolAllocs = (PoolAllocator*)MemoryManager::Alloc(sizeof(PoolAllocator) * allocatorCount, 8);

        for (U32 i = 0; i < allocatorCount; i++)
        {
            m_pPoolAllocs[i] = PoolAllocator(((i + 1) * 2) * blockCount, ((i + 1) * 2), 8);
        }
    }

    void DSManager::Shutdown()
    {
    }

    void* DSManager::Alloc(U64 blockSize)
    {
        for (U32 i = 0; i < m_AllocatorCount; i++)
        {
            if (blockSize <= ((i + 1) * 2))
            {
                return m_pPoolAllocs[i].Alloc();
            }
        }

        return nullptr;
    }

    void DSManager::Free(void* ptr, U64 blockSize)
    {
        for (U32 i = 0; i < m_AllocatorCount; i++)
        {
            if (blockSize <= ((i + 1) * 2))
            {
                m_pPoolAllocs[i].Free(ptr);
                return;
            }
        }
    }

    void* DSManager::AllocBlock(U64 blockSize)
    {
        return m_gpDSManager->Alloc(blockSize);
    }

    void DSManager::FreeBlock(void* ptr, U64 blockSize)
    {
        m_gpDSManager->Free(ptr, blockSize);
    }
}