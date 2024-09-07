#include "Package.h"

#include <File.h>
#include <string.h>
#include <random>
#include "TypeEvaluation.h"

namespace MarkTech
{
	void PackageCompiledAssets(const char* pOutputPath, const char* pPackageName)
	{
		// Package all compiled assets
		char outputPackagePath[MAX_PATH_LENGTH] = "";
		strcpy_s(outputPackagePath, pOutputPath);
		strcat_s(outputPackagePath, pPackageName);
		strcat_s(outputPackagePath, ".mpk");

		File packageFile = FOpen(outputPackagePath, FileAccessType::WRITE); // Create new package file
		U64 offsetToBlob = 0;

		char compiledFilepath[MAX_PATH_LENGTH] = "";
		strcpy_s(compiledFilepath, pOutputPath);
		strcat_s(compiledFilepath, "int\\");

		FileList compiledFileList = FindAllFilesInPath(compiledFilepath); // Get all the compiled assets
		FWrite(&packageFile, (char*)&compiledFileList.listCount, sizeof(U64)); // Write the amount of compiled assets

		// Calculate the offset to blob
		offsetToBlob += sizeof(U64);
		offsetToBlob += (20 + sizeof(AssetType)) * compiledFileList.listCount;

		// Fill package entry list with compiled assets
		for (U32 i = 0; i < compiledFileList.listCount; i++)
		{
			char filepath[MAX_PATH_LENGTH] = "";
			strcpy_s(filepath, compiledFilepath);
			strcat_s(filepath, compiledFileList.ppList[i]);

			// Write asset id
			{
				std::random_device rd;
				std::uniform_int_distribution<U32> dist;
				U32 id = dist(rd); // generate random number
				FWrite(&packageFile, (char*)&id, sizeof(U32)); // write id
			}

			// Write the type and size of the asset and it's offset to blob
			AssetType assetType = EvaluateCompiledAssetType(compiledFileList.ppList[i]);
			File assetFile = FOpen(filepath, FileAccessType::READ);
			FClose(&assetFile);

			FWrite(&packageFile, (char*)&assetType, sizeof(AssetType));
			FWrite(&packageFile, (char*)&assetFile.size, sizeof(U64));
			FWrite(&packageFile, (char*)&offsetToBlob, sizeof(U64));
			offsetToBlob += assetFile.size;
		}

		// Write asset data to file (This is the 'blob' that previous comments were talking about)
		for (U32 i = 0; i < compiledFileList.listCount; i++)
		{
			char filepath[MAX_PATH_LENGTH] = "";
			strcpy_s(filepath, compiledFilepath);
			strcat_s(filepath, compiledFileList.ppList[i]);

			File file = FOpen(filepath, FileAccessType::READ);
			char* pAssetData = (char*)malloc(file.size);
			FRead(&file, pAssetData, file.size);
			FClose(&file);

			FWrite(&packageFile, pAssetData, file.size);
			free(pAssetData);
		}

		FClose(&packageFile);
		FileListFree(&compiledFileList);
	}
}