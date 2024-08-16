#include <File.h>
#include <AssetTypes.h>
#include <AssetCompiler.h>
#include <Package.h>

#ifdef DEBUG
#include <crtdbg.h>
#endif

using namespace MarkTech;

#ifdef MT_PLATFORM_WINDOWS
	HMODULE fileMod = NULL;
#endif

PFN_FindAllFilesInPath FindAllFilesInPath = nullptr;
PFN_FileListFree FileListFree = nullptr;
PFN_GetExtension GetExtension = nullptr;
PFN_GetFilename GetFilename = nullptr;
PFN_AddExtension AddExtension = nullptr;
PFN_AddFilename AddFilename = nullptr;
PFN_ChangeExtension ChangeExtension = nullptr;

PFN_FOpen FOpen = nullptr;
PFN_FClose FClose = nullptr;
PFN_FRead FRead = nullptr;
PFN_FWrite FWrite = nullptr;
PFN_FSeek FSeek = nullptr;

void LinkFileSystem()
{
#ifdef MT_PLATFORM_WINDOWS
	fileMod = LoadLibraryA("FileSystem.dll");
	if (fileMod == NULL)
		return;

	FOpen = (PFN_FOpen)GetProcAddress(fileMod, "FOpen");
	FClose = (PFN_FClose)GetProcAddress(fileMod, "FClose");
	FWrite = (PFN_FWrite)GetProcAddress(fileMod, "FWrite");
	FRead = (PFN_FRead)GetProcAddress(fileMod, "FRead");
	FSeek = (PFN_FSeek)GetProcAddress(fileMod, "FSeek");
	AddFilename = (PFN_AddFilename)GetProcAddress(fileMod, "AddFilename");
	AddExtension = (PFN_AddExtension)GetProcAddress(fileMod, "AddExtension");
	GetExtension = (PFN_GetExtension)GetProcAddress(fileMod, "GetExtension");
	GetFilename = (PFN_GetFilename)GetProcAddress(fileMod, "GetFilename");
	FindAllFilesInPath = (PFN_FindAllFilesInPath)GetProcAddress(fileMod, "FindAllFilesInPath");
	FileListFree = (PFN_FileListFree)GetProcAddress(fileMod, "FileListFree");
	ChangeExtension = (PFN_ChangeExtension)GetProcAddress(fileMod, "ChangeExtension");
#endif
}

void UnlinkFileSystem()
{
#ifdef MT_PLATFORM_WINDOWS
	FreeLibrary(fileMod);
#endif
}

int main(int argc, char* argv[])
{
	// Parse command line args
	char* pInputPath = nullptr;
	char* pOutputPath = nullptr;
	char* pPackageName = nullptr;

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-input") == 0)
		{
			pInputPath = argv[i + 1];
			i++;
			continue;
		}

		if (strcmp(argv[i], "-output") == 0)
		{
			pOutputPath = argv[i + 1];
			i++;
			continue;
		}

		if (strcmp(argv[i], "-name") == 0)
		{
			pPackageName = argv[i + 1];
			i++;
			continue;
		}
	}

	if (!pInputPath || !pOutputPath || !pPackageName)
		return 1;

	// Initialization
	LinkFileSystem();

	// Read asset table
	File assetTableFile = FOpen(pInputPath, FileAccessType::READ);
	char* pAssetTableContents = (char*)malloc(assetTableFile.size + 1);
	FRead(&assetTableFile, pAssetTableContents, assetTableFile.size);
	FClose(&assetTableFile);
	pAssetTableContents[assetTableFile.size] = '\0';

	// Compile all assets
	char* pToken = nullptr;
	char* pNextToken = pAssetTableContents;
	char delimiters[] = "\r\n\0";

	while (pNextToken[0] != '\0')
	{
		pToken = strtok_s(pNextToken, delimiters, &pNextToken); // Get filepath to asset

		// Compile Asset
		CompileAsset(pToken, pOutputPath);
	}

	free(pAssetTableContents); // Free the asset table

	// Package all compiled assets
	PackageCompiledAssets(pOutputPath, pPackageName);

	// Destruction
	UnlinkFileSystem();

#ifdef DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}