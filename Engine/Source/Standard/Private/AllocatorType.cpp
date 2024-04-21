#include "AllocatorType.h"

CAllocator::CAllocator()
{
}

CAllocator::~CAllocator()
{
}

void* CAllocator::Allocate(unsigned int nSizeInBytes)
{
    void* ptr = malloc(nSizeInBytes);
    memset(ptr, 0, nSizeInBytes);
    return ptr;
}

void CAllocator::Deallocate(void* pMemory)
{
    free(pMemory);
}
