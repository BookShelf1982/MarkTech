#include "FileSystem.h"
#ifdef _WINDOWS
#include <Windows.h>
#include <shlwapi.h>
#endif

#ifdef _WINDOWS
void FSAddBackslash(Path* path)
{
	PathAddBackslashA(path->path);
}

void FSAddExtension(Path* path, const char* ext)
{
	PathAddExtensionA(path->path, ext);
}

void FSAppend(Path* path, const char* str)
{
	PathAppendA(path->path, str);
}

char* FSFindExtension(Path* path)
{
	return PathFindExtensionA(path->path);
}

char* FSFindFilename(Path* path)
{
	return PathFindFileNameA(path->path);
}

unsigned char FSIsDirectory(Path* path)
{
	return PathIsDirectoryA(path->path);
}

unsigned char FSPathExists(Path* path)
{
	return PathFileExistsA(path->path);
}

void FSRemoveTrailing(Path* path)
{
	PathRemoveFileSpecA(path->path);
}

void FSGetAbsolutePath(Path* path)
{
	GetFullPathNameA(path->path, sizeof(path->path), path->path, NULL);
}

void FSRenameExtension(Path* path, const char* extension)
{
	PathRenameExtensionA(path->path, extension);
}

void FSOpen(Path* path, OpenFileType openType, File* file)
{
	GetFullPathNameA(path->path, sizeof(path->path), path->path, NULL);
	if (openType == OPEN_TYPE_WRITE)
	{
		Path dirPath = *path;
		FSRemoveTrailing(&dirPath);
		if (PathFileExistsA(dirPath.path) == FALSE)
		{
			if (CreateDirectoryA(dirPath.path, NULL) == FALSE)
			{
				file->size = 0;
				file->_handle = 0;
				return;
			}
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
		access = GENERIC_WRITE;
		behavior = CREATE_ALWAYS;
		share = FILE_SHARE_WRITE;
		break;
	}

	HANDLE hFile = CreateFileA(
		path->path, access,
		share, NULL,
		behavior, FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		file->size = 0;
		file->_handle = 0;
		return;
	}

	LARGE_INTEGER size;
	GetFileSizeEx(hFile, &size);
	file->size = size.QuadPart;
	file->_handle = hFile;
}

size_t FSRead(File* file, char* buffer, size_t size)
{
	size_t bytesRead = 0;
	ReadFile(file->_handle, buffer, (DWORD)size, (DWORD*)&bytesRead, NULL);
	return bytesRead;
}

void FSWrite(File* file, char* buffer, size_t size)
{
	WriteFile(file->_handle, buffer, (DWORD)size, NULL, NULL);
}

void FSSeek(File* file, size_t offset, SeekOrigin origin)
{
	SetFilePointer(file->_handle, *(PDWORD)(&offset), (PLONG)((char*)&offset + 4), origin);
}

void FSClose(File* file)
{
	CloseHandle(file->_handle);
}

unsigned char FSDelete(const char* filepath)
{
	return DeleteFileA(filepath);
}

unsigned char FSDeleteDirectory(const char* directory)
{
	return RemoveDirectoryA(directory);
}

/*FileIterator FSBeginFileIt(const char* directory, FileInfo* info)
{
	WIN32_FIND_DATAA data;
	FileIterator it = FindFirstFileA(directory, &data);
	if (it == INVALID_HANDLE_VALUE)
		return nullptr;

	strcpy_s(info->name, data.cFileName);
	return it;
}

unsigned char FSFileIterate(FileIterator it, FileInfo* info)
{
	WIN32_FIND_DATAA data;
	if (!FindNextFileA(it, &data))
		return false;

	strcpy_s(info->name, data.cFileName);
	return true;
}

void FSEndIt(FileIterator it)
{
	FindClose(it);
}*/
#endif