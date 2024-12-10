#pragma once
#include <PrimitiveTypes.h>

#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace MarkTech
{
	struct PathBuilder
	{
		char* pPath;
	};

	PathBuilder BeginPathBuilder();
	void 
}