#pragma once
#include "Log.h"

#ifdef MT_CORELIB
#ifdef MT_ENABLE_ASSERTS
void BreakToDebugger();
#ifdef MT_PLATFORM_WINDOWS
#define MT_ASSERT(x)	if (x) { }\
					else { MarkTech::DebugPrintF("ASSERT FAILED\nExpression: %s\nLine: %u\nFile: %s\n", #x, __LINE__, __FILE__); BreakToDebugger(); }
#endif
#else
#define MT_ASSERT(x)
#endif

#else
#ifdef MT_ENABLE_ASSERTS
typedef void (*PFN_BreakToDebugger)();
extern PFN_BreakToDebugger BreakToDebugger;
#ifdef MT_PLATFORM_WINDOWS
#define MT_ASSERT(x)	if (x) { }\
					else { DebugPrintF("ASSERT FAILED\nExpression: %s\nLine: %u\nFile: %s\n", #x, __LINE__, __FILE__); BreakToDebugger(); }
#endif
#else
#define MT_ASSERT(x)
#endif
#endif