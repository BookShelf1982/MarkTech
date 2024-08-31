#pragma once
#include <PrimitiveTypes.h>
#include <DLLUTLS.h>
#include "Path.h"
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace MarkTech
{
	enum class FileAccessType
	{
		READ,
		WRITE,
		READ_WRITE
	};

	enum class SeekOrigin
	{
		BEGINING,
		CURRENT,
		END
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

	File FOpen(const char* pFilepath, FileAccessType accessType);
	void FClose(File* pFile);
	void FRead(const File* pFile, char* pBuffer, U64 bytesToRead);
	void FWrite(const File* pFile, char* pBuffer, U64 bytesToRead);
	void FSeek(const File* pFile, I32 location, SeekOrigin origin = SeekOrigin::CURRENT);
}