#pragma once
#include <PrimitiveTypes.h>
#include <DLLUTLS.h>
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace MarkTech
{
	enum class PathType 
	{
		ABSOLUTE_PATH,
		RELATIVE_PATH
	};

	struct Path
	{
		char pPath[260];
		PathType type;
	};

	struct FileList
	{
		char** ppList;
		U64 listCount;
	};

#ifdef MT_FILESYSTEMLIB
	MT_DLLEXPORT FileList FindAllFilesInPath(Path* path);
	MT_DLLEXPORT void FileListFree(FileList* fileList);

	MT_DLLEXPORT Path MakePath(char* pPath);
	MT_DLLEXPORT void MakeAbsolutePath(Path* pPath);
}
#else
}
typedef MarkTech::FileList (*PFN_FindAllFilesInPath)(MarkTech::Path* pPath);
typedef void (*PFN_FileListFree)(MarkTech::FileList* fileList);
typedef MarkTech::Path (*PFN_MakePath)(char* pPath);
typedef void (*PFN_MakeAbsolutePath)(MarkTech::Path* pPath);

extern PFN_FindAllFilesInPath FindAllFilesInPath;
extern PFN_FileListFree FileListFree;
extern PFN_MakePath MakePath;
extern PFN_MakeAbsolutePath MakeAbsolutePath;
#endif