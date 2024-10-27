#include "Log.h"
#include <stdio.h>
#include <File.h>
#include <StackAllocator.h>
#include <HighResTimer.h>
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace MarkTech
{
	File gLogFile = {};
	StackAllocator gConsoleBuffer = {};

	bool InitLog(U64 bufferSize)
	{
		if (!bufferSize)
			return false;

		CreateStackAllocator(&gConsoleBuffer, bufferSize);
		return true;
	}

	void ShutdownLog()
	{
		File logFile;
		FOpen(&logFile, "log.txt", FILE_ACCESS_WRITE);
		if (logFile.isOpened)
			FWrite(&logFile, gConsoleBuffer.pMemory, strlen(gConsoleBuffer.pMemory));
		FreeStackAllocator(&gConsoleBuffer);
	}

	void ConsoleLog(const char* msg, ...)
	{
		va_list argList;
		va_start(argList, msg);

		const U32 MAX_CHARS = 1024;
		char buffer[MAX_CHARS];
		vsnprintf(buffer, MAX_CHARS, msg, argList);

#ifdef MT_PLATFORM_WINDOWS
		OutputDebugStringA(buffer);
#endif
		va_end(argList);
		
		char newBuffer[MAX_CHARS];
		U64 time = (GetCurrentTimestamp() - gStartTimestamp) * 1000 / GetCPUFrequency();

		sprintf_s(newBuffer, "[LOG @ %llums]: %s\n", time, buffer);

		U64 length = strlen(newBuffer);
		char* ptr = (char*)AllocFromStack(&gConsoleBuffer, length);
		strcpy_s(ptr, length + 1, newBuffer);
	}
}