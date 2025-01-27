#include "FileSystem.h"
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#include <shlwapi.h>
#endif

#define DLLEXPORT extern "C" __declspec(dllexport)

DLLEXPORT File FSOpen(const char* filepath, OpenFileType openType)
{
	if (!PathFileExistsA(filepath))
	{
		size_t filepathLength = strlen(filepath);
		for (int i = filepathLength; i > 0; i--)
		{
			if (filepath[i] == '\\')
			{
				filepathLength = i;
				break;
			}
		}

		char* dir = (char*)malloc(filepathLength + 1);
		memcpy_s(dir, filepathLength + 1, filepath, filepathLength);
		if (!CreateDirectoryA(dir, NULL))
			return NULL;
	}

	DWORD access = 0;
	switch (openType)
	{
	case OPEN_TYPE_READ:
		access = GENERIC_READ;
		break;
	case OPEN_TYPE_WRITE:
		access = GENERIC_WRITE;
		break;
	}

	HANDLE file = CreateFileA(
		filepath, access,
		0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	return file;
}

DLLEXPORT void FSClose(File file)
{
	CloseHandle(file);
}