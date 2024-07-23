#include <File.h>
#include <Path.h>
#include <MemoryArena.h>
#include <Package.h>
#include <TextFile.h>
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
PFN_MakePath MakePath = nullptr;
PFN_MakeAbsolutePath MakeAbsolutePath = nullptr;

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

	FindAllFilesInPath = (PFN_FindAllFilesInPath)GetProcAddress(fileMod, "FindAllFilesInPath");
	FileListFree = (PFN_FileListFree)GetProcAddress(fileMod, "FileListFree");
	MakePath = (PFN_MakePath)GetProcAddress(fileMod, "MakePath");
	MakeAbsolutePath = (PFN_MakeAbsolutePath)GetProcAddress(fileMod, "MakeAbsolutePath");
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

int main()
{
	LinkCore();
	LinkFileSystem();

	InitMemoryArena(MEGABYTE * 2);

	Path searchPath = MakePath(".\\");

	FileList list = FindAllFilesInPath(&searchPath);
	FileListFree(&list);

	TextFile file = ReadTextFile("text1.txt");
	PackageEntry entry = {};
	entry.entryId = 1;
	entry.entrySize = file.length;
	entry.entryType = EntryType::ANSI;
	entry.pData = file.pBuffer;

	Package pack = {};
	AddPackageEntry(&pack, entry);
	WritePackageToFile(&pack, "OutputCool.mpk");

	KillMemoryArena();

	UnlinkFileSystem();
	UnlinkCore();

#ifdef DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}