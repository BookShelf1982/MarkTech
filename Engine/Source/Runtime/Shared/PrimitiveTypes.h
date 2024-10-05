#pragma once

/*
* Primitive typedefs
* Author: Markos
*/

#define KILOBYTE 1024
#define MEGABYTE 1048576

#define U8_MAX 0xffUi8
#define U16_MAX 0xfffUi16
#define U32_MAX 0xffffffffui32
#define U64_MAX 0xffffffffffffffffui64

#ifdef _MSC_VER
namespace MarkTech
{
	typedef __int8	I8;
	typedef __int16 I16;
	typedef __int32 I32;
	typedef __int64 I64;

	typedef unsigned __int8	 U8;
	typedef unsigned __int16 U16;
	typedef unsigned __int32 U32;
	typedef unsigned __int64 U64;

	typedef float F32;
	typedef double F64;

	typedef unsigned __int64  uintptr;

	typedef unsigned __int64 ByteSize;
}
#endif
