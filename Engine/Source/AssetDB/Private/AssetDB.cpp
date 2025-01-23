#include "AssetDB.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void WriteAssetEntryFile(const AssetEntryInfo* info)
{
	FILE* file = NULL;
	size_t outputFilepathLen = strlen(info->name) + strlen(info->outputPath) + 5;
	char* outputFilepath = (char*)malloc(outputFilepathLen);
	
	strcpy_s(outputFilepath, outputFilepathLen, info->outputPath);
	strcat_s(outputFilepath, outputFilepathLen, info->name);
	strcat_s(outputFilepath, outputFilepathLen, ".mae");

	fopen_s(&file, outputFilepath, "wb");
	if (!file)
	{
		free(outputFilepath);
		return;
	}

	fwrite((char*)&info->type, sizeof(info->type), 1, file);
	fwrite((char*)&info->id, sizeof(info->id), 1, file);
	size_t nameLen = strlen(info->name);
	fwrite((char*)&nameLen, sizeof(uint32_t), 1, file);
	fwrite((char*)&info->name, nameLen, 1, file);
	size_t pathLen = strlen(info->path);
	fwrite((char*)&pathLen, sizeof(uint32_t), 1, file);
	fwrite((char*)&info->path, pathLen, 1, file);

	fclose(file);
	free(outputFilepath);
}
