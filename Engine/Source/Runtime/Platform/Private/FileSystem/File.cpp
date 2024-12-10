#include "File.h"
#include <Shlwapi.h>

namespace MarkTech
{
#ifdef MT_PLATFORM_WINDOWS
	void FOpen(File* pFile, const char* pFilepath, FileAccessType accessType)
	{
		// Create directory if it doesn't exist
		{
			char filepath[MAX_PATH_LENGTH] = "";
			strcpy_s(filepath, pFilepath);
			char folder[MAX_PATH_LENGTH] = "";
			char* end;

			end = strchr(filepath, '\\');

			while (end != NULL)
			{
				strncpy_s(folder, filepath, end - filepath + 1);
				if (!CreateDirectoryA(folder, NULL))
				{
					DWORD err = GetLastError();

					if (err != ERROR_ALREADY_EXISTS)
					{
						// do whatever handling you'd like
					}
				}
				end = strchr(++end, L'\\');
			}
		}

		pFile->accessType = accessType;

		DWORD accessTypeArg = 0;
		DWORD openMode = 0;
		switch (accessType)
		{
		case FILE_ACCESS_READ:
			accessTypeArg = GENERIC_READ;
			openMode = OPEN_EXISTING;
			break;
		case FILE_ACCESS_WRITE:
			accessTypeArg = GENERIC_WRITE;
			openMode = CREATE_NEW;
			break;
		case FILE_ACCESS_READ_WRITE:
			accessTypeArg = GENERIC_READ | GENERIC_WRITE;
			openMode = OPEN_ALWAYS;
			break;
		}

		pFile->hFile = CreateFileA(
			pFilepath,
			accessTypeArg,
			0,
			NULL,
			openMode,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (pFile->hFile == INVALID_HANDLE_VALUE || GetLastError() == ERROR_FILE_NOT_FOUND)
			pFile->isOpened = false;
		else
			pFile->isOpened = true;

		if (GetLastError() == ERROR_FILE_EXISTS && accessType == FILE_ACCESS_WRITE)
		{
			DeleteFileA(pFilepath);
			FOpen(pFile, pFilepath, accessType);
			return;
		}

		LARGE_INTEGER size = {};
		if (!GetFileSizeEx(pFile->hFile, &size))
			DWORD err = GetLastError();

		pFile->size = size.QuadPart;
	}

	void FClose(File* pFile)
	{
		pFile->isOpened = false;
		CloseHandle(pFile->hFile);
	}

	void FRead(const File* pFile, char* pBuffer, U64 bytesToRead)
	{
		if (pFile->accessType == FILE_ACCESS_WRITE || pFile->isOpened == false)
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
		if (pFile->accessType == FILE_ACCESS_READ || pFile->isOpened == false)
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
		if (pFile->accessType == FILE_ACCESS_WRITE || pFile->isOpened == false)
			return;

		DWORD moveMethod = FILE_CURRENT;

		switch (origin)
		{
		case SEEK_ORIGIN_BEGINING:
			moveMethod = FILE_BEGIN;
			break;
		case SEEK_ORIGIN_CURRENT:
			moveMethod = FILE_CURRENT;
			break;
		case SEEK_ORIGIN_END:
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