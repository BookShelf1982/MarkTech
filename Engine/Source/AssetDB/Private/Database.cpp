#include "Database.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

	FILE* file = NULL;
	fopen_s(&file, path, "wb");
	if (!file)
	{
		free(path);
		return 1;
	}

	fclose(file);
	free(path);
	return 0;
}

void RemoveAsset(const char* name)
{
}
