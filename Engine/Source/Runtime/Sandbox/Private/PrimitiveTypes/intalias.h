#pragma once
#include <stdint.h>

#define KIBIBYTE 1024
#define MEBIBYTE 1048576

namespace MarkTech
{
	typedef int8_t I8;
	typedef int16_t I16;
	typedef int32_t I32;
	typedef int64_t I64;

	typedef uint8_t	U8;
	typedef uint16_t U16;
	typedef uint32_t U32;
	typedef uint64_t U64;

	union I32F32
	{
		I32 i;
		float f;
	};
}