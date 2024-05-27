#include "Memory/AlignedAllocator.h"
#include <stdlib.h>
#include <memory.h>

namespace MarkTech
{
    uintptr AlignAddress(uintptr addr, size_t align)
    {
        const size_t mask = align - 1;
        //assert((align & mask) == 0); // pwr of 2
        return (addr + mask) & ~mask;
    }

    void* AllocAligned(size_t bytes, size_t align)
    {
        // Allocate 'align' more bytes than we need.
        size_t actualBytes = bytes + align;

        // Allocate unaligned block.
        U8* pRawMem = (U8*)malloc(actualBytes);
        memset(pRawMem, 0, actualBytes);

        // Align the block. If no alignment occurred,
        // shift it up the full 'align' bytes so we
        // always have room to store the shift.
        U8* pAlignedMem = AlignPointer(pRawMem, align);
        if (pAlignedMem == pRawMem)
            pAlignedMem += align;

        // Determine the shift, and store it.
        // (This works for up to 256-byte alignment.)
        ptrdiff_t shift = pAlignedMem - pRawMem;
        
        //assert(shift > 0 && shift <= 256);

        pAlignedMem[-1] = static_cast<U8>(shift & 0xFF);
        return pAlignedMem;

    }

    void FreeAligned(void* pMem)
    {
        if (pMem)
        {
            // Convert to U8 pointer.
            U8* pAlignedMem = reinterpret_cast<U8*>(pMem);

            // Extract the shift.
            ptrdiff_t shift = pAlignedMem[-1];
            if (shift == 0)
                shift = 256;
            
            // Back up to the actual allocated address,
            // and array-delete it.
            U8* pRawMem = pAlignedMem - shift;
            free(pRawMem);
        }
    }


}