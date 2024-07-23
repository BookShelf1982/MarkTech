#pragma once
#include <PrimitiveTypes.h>
#include <DLLUTLS.h>
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

#ifdef MT_FILESYSTEMLIB
	MT_DLLEXPORT File FOpen(const char* pFilepath, FileAccessType accessType);
	//MT_DLLEXPORT File FOpen(const char* pFilepath, FileAccessType accessType);
	MT_DLLEXPORT void FClose(File* pFile);
	MT_DLLEXPORT void FRead(const File* pFile, char* pBuffer, U64 bytesToRead);
	MT_DLLEXPORT void FWrite(const File* pFile, char* pBuffer, U64 bytesToRead);
	MT_DLLEXPORT void FSeek(const File* pFile, I32 location, SeekOrigin origin = SeekOrigin::CURRENT);
}
#else
}
	typedef MarkTech::File (*PFN_FOpen)(const char* pFilepath, MarkTech::FileAccessType accessType);
	typedef void (*PFN_FClose)(MarkTech::File* pFile);
	typedef void (*PFN_FRead)(const MarkTech::File* pFile, char* pBuffer, MarkTech::U64 bytesToRead);
	typedef void (*PFN_FWrite)(const MarkTech::File* pFile, char* pBuffer, MarkTech::U64 bytesToRead);
	typedef void (*PFN_FSeek)(const MarkTech::File* pFile, MarkTech::I32 location, MarkTech::SeekOrigin origin);

	extern PFN_FOpen FOpen;
	extern PFN_FClose FClose;
	extern PFN_FRead FRead;
	extern PFN_FWrite FWrite;
	extern PFN_FSeek FSeek;

#endif