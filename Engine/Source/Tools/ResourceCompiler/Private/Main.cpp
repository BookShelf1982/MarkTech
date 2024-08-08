#include <File.h>
#include <AssetTypes.h>
#include <TextFile.h>

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
		AssetType assetType = EvaluateAssetType(pToken);
		char outputFilePath[MAX_PATH_LENGTH] = "";
		strcpy_s(outputFilePath, pOutputPath);
		strcat_s(outputFilePath, "int\\");
		AddFilename(outputFilePath, GetFilename(pToken));

		switch (assetType)
		{
		case AssetType::ANSI_TEXT_FILE:
		{
			CompileTextFile(outputFilePath, pToken);
		}
		break;
		case AssetType::TEXTURE2D:
		{

		}
		break;
		}
	}

	free(pAssetTableContents); // Free the asset table

	// Package all compiled assets
	char outputPackagePath[MAX_PATH_LENGTH] = "";
	strcpy_s(outputPackagePath, pOutputPath);
	strcat_s(outputPackagePath, pPackageName);
	strcat_s(outputPackagePath, ".mpk");

	File packageFile = FOpen(outputPackagePath, FileAccessType::WRITE); // Create new package file
	U64 offsetToBlob = 0;

	char compiledFilepath[MAX_PATH_LENGTH] = "";
	strcpy_s(compiledFilepath, pOutputPath);
	strcat_s(compiledFilepath, "int\\");

	FileList compiledFileList = FindAllFilesInPath(compiledFilepath); // Get all the compiled assets
	FWrite(&packageFile, (char*)&compiledFileList.listCount, sizeof(U64)); // Write the amount of compiled assets

	// Calculate the offset to blob
	offsetToBlob += sizeof(U64);
	offsetToBlob += 18 * compiledFileList.listCount;
	for (U32 i = 0; i < compiledFileList.listCount; i++)
	{
		offsetToBlob += strlen(compiledFileList.ppList[i]);
	}
	
	// Fill package entry list with compiled assets
	for (U32 i = 0; i < compiledFileList.listCount; i++)
	{
		char filepath[MAX_PATH_LENGTH] = "";
		strcpy_s(filepath, compiledFilepath);
		strcat_s(filepath, compiledFileList.ppList[i]);

		// Write asset name
		U16 size = (U16)strlen(compiledFileList.ppList[i]);
		FWrite(&packageFile, (char*)&size, sizeof(U16)); // Write the name length
		FWrite(&packageFile, compiledFileList.ppList[i], size); // Write the name

		// Write the size of the asset and it's offset to blob
		File assetFile = FOpen(filepath, FileAccessType::READ);
		FClose(&assetFile);

		FWrite(&packageFile, (char*)&assetFile.size, sizeof(U64));
		FWrite(&packageFile, (char*)&offsetToBlob, sizeof(U64));
		offsetToBlob += assetFile.size;
	}

	// Write asset data to file (This is the 'blob' that previous comments were talking about)
	for (U32 i = 0; i < compiledFileList.listCount; i++)
	{
		char filepath[MAX_PATH_LENGTH] = "";
		strcpy_s(filepath, compiledFilepath);
		strcat_s(filepath, compiledFileList.ppList[i]);

		File file = FOpen(filepath, FileAccessType::READ);
		char* pAssetData = (char*)malloc(file.size);
		FRead(&file, pAssetData, file.size);
		FClose(&file);

		FWrite(&packageFile, pAssetData, file.size);
		free(pAssetData);
	}

	FClose(&packageFile);
	FileListFree(&compiledFileList);

	// Destruction
	UnlinkFileSystem();

#ifdef DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}