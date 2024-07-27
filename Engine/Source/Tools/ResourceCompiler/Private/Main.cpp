#include <File.h>
#include <Path.h>
#include <MemoryArena.h>
#include <Package.h>
#include <TextFile.h>
#include <AssetTable.h>
#ifdef DEBUG
#include <crtdbg.h>
#endif

using namespace MarkTech;

#ifdef MT_PLATFORM_WINDOWS
	HMODULE fileMod = NULL;
	HMODULE coreMod = NULL;
#endif

PFN_FindAllFilesInPath FindAllFilesInPath = nullptr;
PFN_FileListFree FileListFree = nullptr;
PFN_GetExtension GetExtension = nullptr;
PFN_AddExtension AddExtension = nullptr;
PFN_AddFilename AddFilename = nullptr;

PFN_FOpen FOpen = nullptr;
PFN_FClose FClose = nullptr;
PFN_FRead FRead = nullptr;
PFN_FWrite FWrite = nullptr;
PFN_FSeek FSeek = nullptr;

PFN_InitMemoryArena InitMemoryArena = nullptr;
PFN_AllocFromMemoryArena AllocFromMemoryArena = nullptr;
PFN_FreeToPointer FreeToPointer = nullptr;
PFN_KillMemoryArena KillMemoryArena = nullptr;

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
	GetExtension = (PFN_GetExtension)GetProcAddress(fileMod, "GetExtension");
	AddExtension = (PFN_AddExtension)GetProcAddress(fileMod, "AddExtension");
#endif
}

void UnlinkFileSystem()
{
#ifdef MT_PLATFORM_WINDOWS
	FreeLibrary(fileMod);
#endif
}

void LinkCore()
{
#ifdef MT_PLATFORM_WINDOWS
	coreMod = LoadLibraryA("Core.dll");
	if (coreMod == NULL)
		return;

	InitMemoryArena = (PFN_InitMemoryArena)GetProcAddress(coreMod, "InitMemoryArena");
	AllocFromMemoryArena = (PFN_AllocFromMemoryArena)GetProcAddress(coreMod, "AllocFromMemoryArena");
	KillMemoryArena = (PFN_KillMemoryArena)GetProcAddress(coreMod, "KillMemoryArena");
	FreeToPointer = (PFN_FreeToPointer)GetProcAddress(coreMod, "FreeToPointer");
#endif
}

void UnlinkCore()
{
#ifdef MT_PLATFORM_WINDOWS
	FreeLibrary(coreMod);
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

	// Initialization
	LinkCore();
	LinkFileSystem();
	InitMemoryArena(MEGABYTE * 2);

	// Create asset table from file
	AssetTable table = CreateAssetTable(pInputPath);

	Package pack = {};

	// Read all assets from asset table
	for (U64 i = 0; i < table.entryCount; i++)
	{
		AssetType type = EvaluateAssetType(table.pEntries[i].pPath);
		PackageEntry entry = {};
		entry.entryId = table.pEntries[i].id;
		entry.entryType = type;
		AddPackageEntry(&pack, entry);
	}
	
	char outputPath[512] = "";
	strcpy_s(outputPath, pOutputPath);
	AddFilename(outputPath, pPackageName);
	WritePackageToFile(&pack, outputPath);


	KillMemoryArena();

	UnlinkFileSystem();
	UnlinkCore();

#ifdef DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}