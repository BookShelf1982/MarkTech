#include "Database.h"

#include <FileSystem\FileSystem.h>
#include <string.h>
#include <stdlib.h>

int AddAsset(const char* name, const char* srcAssetPath, const char* databasePath)
{
	size_t dbPathLength = strlen(databasePath);
	size_t pathLength = strlen(name) + dbPathLength + 6;
	char* path = (char*)malloc(pathLength);
	if (databasePath[dbPathLength - 1] != '\\' && databasePath[dbPathLength - 1] != '/')
	{
		strcpy_s(path, pathLength, databasePath);
		path[dbPathLength] = '\\';
	}
	else
		strcpy_s(path, pathLength, databasePath);

	strcat_s(path, pathLength, name);
	strcat_s(path, pathLength, ".xml");

	File file = FSOpen(path, OPEN_TYPE_WRITE);
	if (!file._handle)
	{
		free(path);
		return 1;
	}

	FSClose(&file);
	free(path);
	return 0;
}

int RemoveAsset(const char* assetPath)
{
	return !FSDelete(assetPath);
}
