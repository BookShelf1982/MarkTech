#pragma once
#include <PrimitiveTypes.h>

namespace MarkTech
{
	bool InitLog(U64 bufferSize);
	void ShutdownLog();
	void ConsoleLog(const char* pFmt, ...);
}