#pragma once
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
typedef HMODULE DynamicLib;

DynamicLib OpenDL(const char* path)
{
	return LoadLibraryA(path);
}

void CloseDL(DynamicLib obj)
{
	FreeLibrary(obj);
}

void* GetDLProc(DynamicLib obj, const char* procName)
{
	return GetProcAddress(obj, procName);
}
#endif