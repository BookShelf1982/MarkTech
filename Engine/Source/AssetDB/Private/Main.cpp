#include "Database.h"
#include "Compiler.h"
#include "StringHash.h"
#include <DynamicLib\DynamicLib.h>
#include <FileSystem\FileSystem.h>

#include <stdint.h>
#include <string.h>
#include <stdio.h>

/*
* Pfn declarations
*/

DynamicLib g_FSLib = nullptr;
PFN_FSOpen FSOpen = nullptr;
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
	FSOpen = (PFN_FSOpen)GetDLProc(g_FSLib, "FSOpen");
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
*	"skins/player/teddy.spr"
*	"props/generic/barrel.spr"
*	"props/desert/tumblweed.spr"
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
	}

	if (g_FSLib)
		CloseDL(g_FSLib);

	return returnCode;
}