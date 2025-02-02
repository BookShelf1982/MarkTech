#include "FileSystem.h"
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#include <shlwapi.h>
#endif

#define DLLEXPORT extern "C" __declspec(dllexport)

#ifdef MT_PLATFORM_WINDOWS
DLLEXPORT Path FSCreatePath(const char* str)
{
	Path path = {};
	PathCanonicalizeA(path.path, str);
	return path;
}

DLLEXPORT void FSAddBackslash(Path* path)
{
	PathAddBackslashA(path->path);
}

DLLEXPORT void FSAddExtension(Path* path, const char* ext)
{
	PathAddExtensionA(path->path, ext);
}

DLLEXPORT void FSAppend(Path* path, const char* str)
{
	PathAppendA(path->path, str);
}

DLLEXPORT char* FSFindExtension(Path* path)
{
	return PathFindExtensionA(path->path);
}

DLLEXPORT char* FSFindFilename(Path* path)
{
	return PathFindFileNameA(path->path);
}

DLLEXPORT bool FSIsDirectory(Path* path)
{
	return PathIsDirectoryA(path->path);
}

DLLEXPORT void FSRemoveTrailing(Path* path)
{
	PathRemoveFileSpecA(path->path);
}

DLLEXPORT void FSRenameExtension(Path* path, const char* extension)
{
	PathRenameExtensionA(path->path, extension);
}

DLLEXPORT File FSOpenWithPath(Path* path, OpenFileType openType)
{
	File file = {};

	if (openType == OPEN_TYPE_WRITE)
	{
		Path dirPath = *path;
		FSRemoveTrailing(&dirPath);
		if (!PathFileExistsA(dirPath.path))
		{
			CreateDirectoryA(dirPath.path, NULL);
		}
	}

	DWORD access = 0;
	DWORD behavior = 0;
	DWORD share = 0;
	switch (openType)
	{
	case OPEN_TYPE_READ:
		access = GENERIC_READ;
		behavior = OPEN_EXISTING;
		share = FILE_SHARE_READ;
		break;
	case OPEN_TYPE_WRITE:
		access = FILE_GENERIC_WRITE;
		behavior = CREATE_ALWAYS;
		share = FILE_SHARE_WRITE;
		break;
	}

	HANDLE hFile = CreateFileA(
		path->path, access,
		0, NULL,
		behavior, 0,
		NULL
	);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		file.size = 0;
		file._handle = 0;
		return file;
	}

	LARGE_INTEGER size;
	GetFileSizeEx(hFile, &size);
	file.size = size.QuadPart;
	file._handle = hFile;

	return file;
}

DLLEXPORT File FSOpen(const char* filepath, OpenFileType openType)
{
	File file = {};
	Path path = FSCreatePath(filepath);

	file = FSOpenWithPath(&path, openType);
	return file;
}

DLLEXPORT size_t FSRead(File* file, char* buffer, size_t size)
{
	size_t bytesRead = 0;
	ReadFile(file->_handle, buffer, size, (DWORD*)&bytesRead, NULL);
	return bytesRead;
}

DLLEXPORT void FSWrite(File* file, char* buffer, size_t size)
{
	WriteFile(file->_handle, buffer, size, NULL, NULL);
}

DLLEXPORT void FSSeek(File* file, size_t offset, SeekOrigin origin)
{
	SetFilePointer(file->_handle, *(PDWORD)(&offset), (PLONG)((char*)&offset + 4), origin);
}

DLLEXPORT void FSClose(File* file)
{
	CloseHandle(file->_handle);
}

DLLEXPORT bool FSDelete(const char* filepath)
{
	return DeleteFileA(filepath);
}

DLLEXPORT bool FSDeleteDirectory(const char* directory)
{
	return RemoveDirectoryA(directory);
}

DLLEXPORT FileIterator FSBeginFileIt(const char* directory, FileInfo* info)
{
	WIN32_FIND_DATAA data;
	FileIterator it = FindFirstFileA(directory, &data);
	if (it == INVALID_HANDLE_VALUE)
		return nullptr;

	info->name = strdup(data.cFileName);
	return it;
}

DLLEXPORT bool FSFileIterate(FileIterator it, FileInfo* info)
{
	WIN32_FIND_DATAA data;
	free(info->name);
	if (!FindNextFileA(it, &data))
		return false;

	info->name = strdup(data.cFileName);
	return true;
}

DLLEXPORT void FSEndIt(FileIterator it)
{
	FindClose(it);
}
#endif