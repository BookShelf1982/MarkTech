#pragma once
#include <PrimitiveTypes.h>

namespace MarkTech
{
    //! Byte swap unsigned short
    inline U16 SwapU16(U16 val)
    {
        return (val << 8) | (val >> 8);
    }

    //! Byte swap short
    inline I16 SwapI16(I16 val)
    {
        return (val << 8) | ((val >> 8) & 0xFF);
    }

    //! Byte swap unsigned int
    inline U32 SwapU32(U32 val)
    {
        val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
        return (val << 16) | (val >> 16);
    }

    //! Byte swap int
    inline I32 SwapI32(I32 val)
    {
        val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
        return (val << 16) | ((val >> 16) & 0xFFFF);
    }

    I64 SwapI64(I64 val)
    {
        val = ((val << 8) & 0xFF00FF00FF00FF00ULL) | ((val >> 8) & 0x00FF00FF00FF00FFULL);
        val = ((val << 16) & 0xFFFF0000FFFF0000ULL) | ((val >> 16) & 0x0000FFFF0000FFFFULL);
        return (val << 32) | ((val >> 32) & 0xFFFFFFFFULL);
    }

    U64 SwapU64(U64 val)
    {
        val = ((val << 8) & 0xFF00FF00FF00FF00ULL) | ((val >> 8) & 0x00FF00FF00FF00FFULL);
        val = ((val << 16) & 0xFFFF0000FFFF0000ULL) | ((val >> 16) & 0x0000FFFF0000FFFFULL);
        return (val << 32) | (val >> 32);
    }
}