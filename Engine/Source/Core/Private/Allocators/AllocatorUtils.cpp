#include "Allocators\AllocatorUtils.h"

#include <stdlib.h>

namespace MarkTech
{
    void* MarkTech::AlignedAlloc(size_t size, size_t alignment)
    {
        return _aligned_malloc(size, alignment);
    }
    
    void MarkTech::AlignedFree(void* ptr)
    {
        _aligned_free(ptr);
    }
}