#include "Path.h"
#ifdef MT_PLATFORM_WINDOWS
#include <Shlwapi.h>
#endif

namespace MarkTech
{
#ifdef MT_PLATFORM_WINDOWS
	FileList FindAllFilesInPath(const char* pPath)
	{
		FileList fileList = {};

		char pDirCpy[MAX_PATH] = "";

		GetFullPathNameA(pPath, MAX_PATH, pDirCpy, NULL);

		strcat_s(pDirCpy, "\\*");

		WIN32_FIND_DATAA findData;
		HANDLE hFind;
		hFind = FindFirstFileA(pDirCpy, &findData);

		if (hFind == INVALID_HANDLE_VALUE)
			return fileList;

		U64 numOfFiles = 0;
		while (FindNextFileA(hFind, &findData) != 0)
		{
			if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				numOfFiles++;
			}
		}

		if (numOfFiles == 0)
			return fileList;

		char** ppFileList = (char**)malloc(sizeof(char**) * numOfFiles);

		hFind = FindFirstFileA(pDirCpy, &findData);

		U64 index = 0;
		while (FindNextFileA(hFind, &findData) != 0)
		{
			if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				char* pFilename = (char*)malloc(MAX_PATH);
				strcpy_s(pFilename, MAX_PATH, findData.cFileName);

				ppFileList[index] = pFilename;
				index++;
			}
		}

		FindClose(hFind);

		fileList.listCount = numOfFiles;
		fileList.ppList = ppFileList;

		return fileList;
	}

	void FileListFree(FileList* fileList)
	{
		if (!fileList->ppList)
			return;

		for (U64 i = 0; i < fileList->listCount; i++)
		{
			free(fileList->ppList[i]);
		}

		free(fileList->ppList);
	}

	void AddFilename(char* pPath, const char* pFilename)
	{
		PathAppendA(pPath, pFilename);
	}

	void AddExtension(char* pPath, const char* pExtension)
	{
		PathAddExtensionA(pPath, pExtension);
	}
	
	char* GetExtension(const char* pFilePath)
	{
		return PathFindExtensionA(pFilePath);
	}

	char* GetFilename(const char* pFilePath)
	{
		return PathFindFileNameA(pFilePath);
	}
	
	void ChangeExtension(char* pPath, const char* pExtension)
	{
		PathRemoveExtensionA(pPath);
		PathAddExtensionA(pPath, pExtension);
	}
#endif
}