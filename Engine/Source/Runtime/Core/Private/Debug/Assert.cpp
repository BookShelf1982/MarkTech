#include "Assert.h"
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#ifdef MT_ENABLE_ASSERTS
#ifdef MT_PLATFORM_WINDOWS
void BreakToDebugger()
{
	DebugBreak();
}
#endif
#endif
