#include "PrimitiveTypes\intalias.h"

namespace MarkTech
{
	typedef U32 StringId;
	constexpr StringId operator""_sid(const char* str, size_t size)
	{
		uint32_t hash = 5381;
		int32_t c = 0;

		while (c = *str++)
			hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

		return hash;
	}
}