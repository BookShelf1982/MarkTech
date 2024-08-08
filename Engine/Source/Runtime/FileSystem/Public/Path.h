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

#ifdef MT_FILESYSTEMLIB
	MT_DLLEXPORT FileList FindAllFilesInPath(const char* path);
	MT_DLLEXPORT void FileListFree(FileList* fileList);
	MT_DLLEXPORT void AddFilename(char* pPath, const char* pFilename);
	MT_DLLEXPORT void AddExtension(char* pPath, const char* pExtension);
	MT_DLLEXPORT char* GetExtension(const char* pFilePath);
	MT_DLLEXPORT char* GetFilename(const char* pFilePath);
	MT_DLLEXPORT void ChangeExtension(char* pPath, const char* pExtension);
}
#else
}
typedef MarkTech::FileList (*PFN_FindAllFilesInPath)(const char* pPath);
typedef void (*PFN_FileListFree)(MarkTech::FileList* fileList);
typedef void (*PFN_AddFilename)(char* pPath, const char* pFilename);
typedef void (*PFN_AddExtension)(char* pPath, const char* pExtension);
typedef char* (*PFN_GetExtension)(const char* pFilePath);
typedef char* (*PFN_GetFilename)(const char* pFilePath);
typedef void (*PFN_ChangeExtension)(char* pPath, const char* pExtension);

extern PFN_FindAllFilesInPath FindAllFilesInPath;
extern PFN_FileListFree FileListFree;
extern PFN_AddFilename AddFilename;
extern PFN_AddExtension AddExtension;
extern PFN_GetExtension GetExtension;
extern PFN_GetFilename GetFilename;
extern PFN_ChangeExtension ChangeExtension;
#endif