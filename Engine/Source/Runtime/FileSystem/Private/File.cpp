#include "File.h"
#include <Shlwapi.h>

namespace MarkTech
{
#ifdef MT_PLATFORM_WINDOWS
	File FOpen(const char* pFilepath, FileAccessType accessType)
	{
		{ // Create directory if it doesn't exist
			char path[512] = "";
			strcpy_s(path, pFilepath);
			PathRemoveFileSpecA(path);
			if (!PathFileExistsA(path))
				CreateDirectoryA(path, nullptr);
		}

		File file = {};
		file.accessType = accessType;

		DWORD accessTypeArg = 0;
		DWORD openMode = 0;
		switch (accessType)
		{
		case FileAccessType::READ:
			accessTypeArg = GENERIC_READ;
			openMode = OPEN_EXISTING;
			break;
		case FileAccessType::WRITE:
			accessTypeArg = GENERIC_WRITE;
			openMode = CREATE_NEW;
			break;
		case FileAccessType::READ_WRITE:
			accessTypeArg = GENERIC_READ | GENERIC_WRITE;
			openMode = OPEN_ALWAYS;
			break;
		}

		file.hFile = CreateFileA(
			pFilepath,
			accessTypeArg,
			0,
			NULL,
			openMode,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (file.hFile == INVALID_HANDLE_VALUE || GetLastError() == ERROR_FILE_NOT_FOUND)
			file.isOpened = false;
		else
			file.isOpened = true;

		LARGE_INTEGER size = {};
		if (!GetFileSizeEx(file.hFile, &size))
			DWORD err = GetLastError();

		file.size = size.QuadPart;
		
		return file;
	}

	void FClose(File* pFile)
	{
		pFile->isOpened = false;
		CloseHandle(pFile->hFile);
	}

	void FRead(const File* pFile, char* pBuffer, U64 bytesToRead)
	{
		if (pFile->accessType == FileAccessType::WRITE || pFile->isOpened == false)
			return;

		ReadFile(
			pFile->hFile,
			pBuffer,
			(U32)bytesToRead,
			NULL,
			NULL
		);
	}

	void FWrite(const File* pFile, char* pBuffer, U64 bytesToRead)
	{
		if (pFile->accessType == FileAccessType::READ || pFile->isOpened == false)
			return;

		WriteFile(
			pFile->hFile,
			pBuffer,
			(U32)bytesToRead,
			NULL,
			NULL
		);
	}

	void FSeek(const File* pFile, I32 location, SeekOrigin origin)
	{
		if (pFile->accessType == FileAccessType::WRITE || pFile->isOpened == false)
			return;

		DWORD moveMethod = FILE_CURRENT;

		switch (origin)
		{
		case MarkTech::SeekOrigin::BEGINING:
			moveMethod = FILE_BEGIN;
			break;
		case MarkTech::SeekOrigin::CURRENT:
			moveMethod = FILE_CURRENT;
			break;
		case MarkTech::SeekOrigin::END:
			moveMethod = FILE_END;
			break;
		default:
			moveMethod = FILE_CURRENT;
			break;
		}

		SetFilePointer(
			pFile->hFile,
			location,
			NULL,
			moveMethod
		);
	}
#endif
}