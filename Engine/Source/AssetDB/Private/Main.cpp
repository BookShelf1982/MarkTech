#include "Database.h"
#include "Compiler.h"
#include "Packager.h"
#include "StringHash.h"
#include <DynamicLib\DynamicLib.h>
#include <FileSystem\FileSystem.h>

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <crtdbg.h>

/*
* Pfn declarations
*/

DynamicLib g_FSLib = nullptr;
PFN_FSCreatePath FSCreatePath = nullptr;
PFN_FSRemoveTrailing FSRemoveTrailing = nullptr;
PFN_FSAddExtension FSAddExtension = nullptr;
PFN_FSRenameExtension FSRenameExtension = nullptr;
PFN_FSAppend FSAppend = nullptr;
PFN_FSFindExtension FSFindExtension = nullptr;
PFN_FSFindFilename FSFindFilename = nullptr;
PFN_FSIsDirectory FSIsDirectory = nullptr;
PFN_FSOpen FSOpen = nullptr;
PFN_FSOpenWithPath FSOpenWithPath = nullptr;
PFN_FSClose FSClose = nullptr;
PFN_FSRead FSRead = nullptr;
PFN_FSWrite FSWrite = nullptr;
PFN_FSDelete FSDelete = nullptr;
PFN_FSDeleteDirectory FSDeleteDirectory = nullptr;
PFN_FSBeginFileIt FSBeginFileIt = nullptr;
PFN_FSFileIterate FSFileIterate = nullptr;
PFN_FSEndFileIt FSEndFileIt = nullptr;

bool LinkFileSystem()
{
	g_FSLib = OpenDL("FileSystem.dll");
	if (!g_FSLib)
		return false;
	FSCreatePath = (PFN_FSCreatePath)GetDLProc(g_FSLib, "FSCreatePath");
	FSRemoveTrailing = (PFN_FSRemoveTrailing)GetDLProc(g_FSLib, "FSRemoveTrailing");
	FSAddExtension = (PFN_FSAddExtension)GetDLProc(g_FSLib, "FSAddExtension");
	FSRenameExtension = (PFN_FSRenameExtension)GetDLProc(g_FSLib, "FSRenameExtension");
	FSAppend = (PFN_FSAppend)GetDLProc(g_FSLib, "FSAppend");
	FSFindExtension = (PFN_FSFindExtension)GetDLProc(g_FSLib, "FSFindExtension");
	FSFindFilename = (PFN_FSFindExtension)GetDLProc(g_FSLib, "FSFindFilename");
	FSIsDirectory = (PFN_FSIsDirectory)GetDLProc(g_FSLib, "FSIsDirectory");
	FSOpen = (PFN_FSOpen)GetDLProc(g_FSLib, "FSOpen");
	FSOpenWithPath = (PFN_FSOpenWithPath)GetDLProc(g_FSLib, "FSOpenWithPath");
	FSClose = (PFN_FSClose)GetDLProc(g_FSLib, "FSClose");
	FSRead = (PFN_FSRead)GetDLProc(g_FSLib, "FSRead");
	FSWrite = (PFN_FSWrite)GetDLProc(g_FSLib, "FSWrite");
	FSDelete = (PFN_FSDelete)GetDLProc(g_FSLib, "FSDelete");
	FSDeleteDirectory = (PFN_FSDeleteDirectory)GetDLProc(g_FSLib, "FSDeleteDirectory");
	FSBeginFileIt = (PFN_FSBeginFileIt)GetDLProc(g_FSLib, "FSBeginFileIt");
	FSFileIterate = (PFN_FSFileIterate)GetDLProc(g_FSLib, "FSFileIterate");
	FSEndFileIt = (PFN_FSEndFileIt)GetDLProc(g_FSLib, "FSEndFileIt");
	return true;
}

/*
* Commands:
*	add <asset-name>, <db-path>, <src-asset-path> = Adds an asset to the database
*	remove <asset-path> = Removes an asset from the database
*	compile <asset-path>, <output-path> = Compiles an asset
*	fullcompile <root-path>, <output-path> = Compiles all assets in root path
*	package <root-path>, <output-path> = Packages all assets into a mpk file
* 
* "skins/player/teddy.spr"
* "props/generic/barrel.spr"
* "props/desert/tumblweed.spr"
* "shader.spv"
*/

/*
* Package directory layout:
*	Header:
*		flags: u32
*		version: u32
*	Directory:
*		AssetEntry:
*			id: u32
*			pkgIdx: u32
*			size: u64
*			offset: u64
*/

int main(int argc, char* argv[])
{
	if (!LinkFileSystem())
		return 2;

	int returnCode = 1;
	switch (hashString(argv[1]))
	{
	case "add"_sid: 
	{
		if (argc != 5) 
		{
			printf("Incorrect amount of arguments.");
			return -1;
		}

		returnCode = AddAsset(argv[2], argv[4], argv[3]);
		return returnCode;
	} break;
	case "remove"_sid: 
	{
		if (argc != 3) 
		{
			printf("Incorrect amount of arguments.");
			return -1;
		}

		returnCode = RemoveAsset(argv[2]);
		return returnCode;
	} break;
	case "compile"_sid: 
	{
		if (argc != 4)
		{
			printf("Incorrect amount of arguments.");
			return -1;
		}

		returnCode = !GenericCompile(argv[2], argv[3]);
		return returnCode;
	} break;
	case "package"_sid:
	{
		if (argc != 4)
		{
			printf("Incorrect amount of arguments.");
			return -1;
		}

		returnCode = !PackageAllFiles(argv[2], argv[3]);
		return returnCode;
	} break;
	}

	if (g_FSLib)
		CloseDL(g_FSLib);

	return returnCode;
}