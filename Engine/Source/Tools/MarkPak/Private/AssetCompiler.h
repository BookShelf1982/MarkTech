#pragma once
#include "DLLUTILS.h"
#include <stdint.h>

#define MAX_PATH_LENGTH 200

enum AssetType
{
	ASSET_TYPE_SPRITE
};

struct CompilerConfig
{
	const char intermediateDir[MAX_PATH_LENGTH];
	const char outputDir[MAX_PATH_LENGTH];
	const char inputDir[MAX_PATH_LENGTH];
};

EXPORT_DLL void CompileAssets(uint32_t fileCount, const char* filepaths[]);