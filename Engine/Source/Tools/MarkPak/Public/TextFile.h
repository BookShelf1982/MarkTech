#pragma once
#include <PrimitiveTypes.h>

namespace MarkTech
{
	struct TextFile
	{
		char* pBuffer;
		U64 length;
	};

	TextFile ReadTextFile(const char* filepath);
}