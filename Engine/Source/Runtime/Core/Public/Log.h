#pragma once
#include <PrimitiveTypes.h>
#include <DLLUTLS.h>

namespace MarkTech
{
#ifdef MT_CORELIB
	MT_DLLEXPORT void DebugPrintF(const char* pFmt, ...);
}
#else
}
	typedef void (*PFN_DebugPrintF)(const char* pFmt, ...);

	extern PFN_DebugPrintF DebugPrintF;
#endif