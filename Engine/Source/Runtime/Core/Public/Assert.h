#pragma once
#include "Log.h"

#ifdef MT_ENABLE_ASSERTS
void BreakToDebugger();
#ifdef MT_PLATFORM_WINDOWS
#define MT_ASSERT(x)	if (x) { }\
					else { MarkTech::DebugPrintF("ASSERT FAILED\nExpression: %s\nLine: %u\nFile: %s\n", #x, __LINE__, __FILE__); BreakToDebugger(); }
#endif
#else
#define MT_ASSERT(x)
#endif