#pragma once
#include <PrimitiveTypes.h>
#include <DLLUTLS.h>
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#define MAX_PATH_LENGTH 512

namespace MarkTech
{
	struct FileList
	{
		char** ppList;
		U64 listCount;
	};

	FileList FindAllFilesInPath(const char* path);
	void FileListFree(FileList* fileList);
	void AddFilename(char* pPath, const char* pFilename);
	void AddExtension(char* pPath, const char* pExtension);
	char* GetExtension(const char* pFilePath);
	char* GetFilename(const char* pFilePath);
	void ChangeExtension(char* pPath, const char* pExtension);
}