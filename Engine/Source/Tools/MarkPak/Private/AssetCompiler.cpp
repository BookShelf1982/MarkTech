#include "AssetCompiler.h"
#include "TypeEvaluation.h"
#include <File.h>
#include <string.h>
#include <stdio.h>

namespace MarkTech
{
	CompilerResult CompileTextFile(AssetTableEntry* pEntry, const char* pOutputPath)
	{
		char outputpath[256] = "";
		char strNumber[16] = "";
		strcpy_s(outputpath, pOutputPath);
		strcat_s(outputpath, "int\\");
		sprintf_s(strNumber, "%u", pEntry->assetId);
		strcat_s(outputpath, strNumber);
		strcat_s(outputpath, ".txt");

		File file = FOpen(pEntry->assetFilepath, FileAccessType::READ);
		if (!file.isOpened)
			return CR_FILE_IO_FAILED;
		char* pData = (char*)malloc(file.size);
		FRead(&file, (char*)pData, file.size);
		FClose(&file);
		File outFile = FOpen(outputpath, FileAccessType::WRITE);
		
		if (!outFile.isOpened)
		{
			free(pData);
			return CR_FILE_IO_FAILED;
		}

		FWrite(&outFile, pData, file.size);
		FClose(&outFile);
		
		free(pData);
		pEntry->assetSize = file.size;
		return CR_SUCCESS;
	}

	CompilerResult CompileShader(AssetTableEntry* pEntry, const char* pOutputPath)
	{
		char outputpath[256] = "";
		char strNumber[16] = "";
		strcpy_s(outputpath, pOutputPath);
		strcat_s(outputpath, "int\\");
		sprintf_s(strNumber, "%u", pEntry->assetId);
		strcat_s(outputpath, strNumber);
		strcat_s(outputpath, ".spv");

		File file = FOpen(pEntry->assetFilepath, FileAccessType::READ);
		if (!file.isOpened)
			return CR_FILE_IO_FAILED;
		char* pData = (char*)malloc(file.size);
		FRead(&file, (char*)pData, file.size);
		FClose(&file);

		File outFile = FOpen(outputpath, FileAccessType::WRITE);

		if (!outFile.isOpened)
		{
			free(pData);
			return CR_FILE_IO_FAILED;
		}

		FWrite(&outFile, pData, file.size);
		FClose(&outFile);

		free(pData);
		pEntry->assetSize = file.size;
		return CR_SUCCESS;
	}

	CompilerResult CompileAsset(AssetTable* pTable, const char* pOutputPath)
	{
		CompilerResult result = CR_SUCCESS;
		for (U32 i = 0; i < pTable->entryCount; i++)
		{
			AssetType type = EvaluateAssetType(pTable->pEntries[i].assetFilepath);

			switch (type)
			{
			case AssetType::ANSI_TEXT_FILE:
				result = CompileTextFile(&pTable->pEntries[i], pOutputPath);
				if (result != CR_SUCCESS)
					return result;
				break;
			case AssetType::EFFECTS:
				break;
			case AssetType::SHADER:
				result = CompileShader(&pTable->pEntries[i], pOutputPath);
				if (result != CR_SUCCESS)
					return result;
				break;
			}

			pTable->pEntries[i].type = type;
		}

		return result;
	}
}