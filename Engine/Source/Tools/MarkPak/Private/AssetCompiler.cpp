#include "AssetCompiler.h"
#include "AssetTypes.h"

#include <Path.h>
#include <string.h>

namespace MarkTech
{
	void CompileAsset(const char* pFilepath, const char* pOutputPath)
	{
		AssetType assetType = EvaluateAssetType(pFilepath);
		char outputFilePath[MAX_PATH_LENGTH] = "";
		strcpy_s(outputFilePath, pOutputPath);
		strcat_s(outputFilePath, "int\\");
		AddFilename(outputFilePath, GetFilename(pFilepath));

		switch (assetType)
		{
		case AssetType::ANSI_TEXT_FILE:
		{
			CompileTextFile(outputFilePath, pFilepath);
		}
		return;
		case AssetType::TEXTURE2D:
		{

		}
		return;
		}
	}
}