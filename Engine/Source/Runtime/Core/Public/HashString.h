#pragma once
#include <PrimitiveTypes.h>

namespace MarkTech
{
	typedef U32 HashString;

	HashString Hash(const char* pStr);
	const char* GetStringFromHash(HashString hash);
}