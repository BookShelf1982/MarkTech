#pragma once
#include <PrimitiveTypes.h>
#include <DLLUTLS.h>
#include "Path.h"
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace MarkTech
{
	enum FileAccessType
	{
		FILE_ACCESS_READ,
		FILE_ACCESS_WRITE,
		FILE_ACCESS_READ_WRITE
	};

	enum SeekOrigin
	{
		SEEK_ORIGIN_BEGINING,
		SEEK_ORIGIN_CURRENT,
		SEEK_ORIGIN_END
	};

	struct File
	{
#ifdef MT_PLATFORM_WINDOWS
		HANDLE hFile;
#endif
		bool isOpened;
		U64 size;
		FileAccessType accessType;
	};

	void FOpen(File* pFile, const char* pFilepath, FileAccessType accessType);
	void FClose(File* pFile);
	void FRead(const File* pFile, char* pBuffer, U64 bytesToRead);
	void FWrite(const File* pFile, char* pBuffer, U64 bytesToRead);
	void FSeek(const File* pFile, I32 location, SeekOrigin origin = SEEK_ORIGIN_CURRENT);
}