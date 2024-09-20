#include "Package.h"
#include "TypeEvaluation.h"
#include <File.h>

#include <stdio.h>

namespace MarkTech
{
	PackageResult PackageCompiledAssets(const PackageCompiledAssetsCreateInfo* pInfo)
	{
		// Write file
		char outputFilepath[MAX_PATH_LENGTH] = "";
		strcpy_s(outputFilepath, pInfo->pOutputPath);
		strcat_s(outputFilepath, pInfo->pPackageName);
		strcat_s(outputFilepath, ".mpk");
		File packageFile = FOpen(outputFilepath, FileAccessType::WRITE);
		if (!packageFile.isOpened)
			return PR_FILE_ERROR;

		U64 offsetToBlob = 0;

		// Write package metadata
		// If pSignature exists then write signature
		if (pInfo->packageMetadata.signature[0] != 0)
		{
			U8 stringLength = (U8)strlen((const char*)pInfo->packageMetadata.signature);
			FWrite(&packageFile, (char*)&stringLength, sizeof(U8));
			FWrite(&packageFile, (char*)pInfo->packageMetadata.signature, stringLength);
			offsetToBlob += (1 + stringLength);
		}
		else
		{
			U8 stringLength = 0;
			FWrite(&packageFile, (char*)&stringLength, sizeof(U8));
			offsetToBlob += 1;
		}

		// Write the number of assets
		FWrite(&packageFile, (char*)&pInfo->pAssetTable->entryCount, sizeof(U64));
		// Write the bitflags
		FWrite(&packageFile, (char*)&pInfo->packageFlags, sizeof(U32));
		offsetToBlob += 12;

		offsetToBlob += (sizeof(U32) + sizeof(AssetType) + sizeof(U64) + sizeof(U64)) * pInfo->pAssetTable->entryCount;

		// If packageFlags has PACKAGE_FLAGS_STRINGTABLE then write string table
		if (pInfo->packageFlags & PACKAGE_FLAGS_STRINGTABLE)
		{
			FWrite(&packageFile, (char*)&pInfo->pAssetTable->entryCount, sizeof(U32)); // write the amount of elements in string table
			offsetToBlob += 4;
			for (U64 i = 0; i < pInfo->pAssetTable->entryCount; i++)
			{
				char filename[128] = "";
				strcpy_s(filename, GetFilename(pInfo->pAssetTable->pEntries[i].assetFilepath));
				ChangeExtension(filename, GetCompiledAssetTypeExtension(pInfo->pAssetTable->pEntries[i].type));
				// Write the asset id
				FWrite(&packageFile, (char*)&pInfo->pAssetTable->pEntries[i].assetId, sizeof(U32));
				offsetToBlob += 4;
				// Write the asset name
				U8 stringLength = (U8)strlen((const char*)filename);
				FWrite(&packageFile, (char*)&stringLength, sizeof(U8));
				FWrite(&packageFile, (char*)filename, stringLength);
				offsetToBlob += (1 + stringLength);
			}
		}

		// Write the package entries
		for (U64 i = 0; i < pInfo->pAssetTable->entryCount; i++)
		{
			FWrite(&packageFile, (char*)&pInfo->pAssetTable->pEntries[i].assetId, sizeof(U32)); // asset id
			FWrite(&packageFile, (char*)&pInfo->pAssetTable->pEntries[i].type, sizeof(AssetType)); // asset type
			FWrite(&packageFile, (char*)&pInfo->pAssetTable->pEntries[i].assetSize, sizeof(U64)); // asset size
			FWrite(&packageFile, (char*)&offsetToBlob, sizeof(U64)); // offset to blob/asset data
			offsetToBlob += pInfo->pAssetTable->pEntries[i].assetSize;
		}

		// Write the asset data
		for (U32 i = 0; i < pInfo->pAssetTable->entryCount; i++)
		{
			char filepath[MAX_PATH_LENGTH] = "";
			char strNumber[16] = "";
			strcpy_s(filepath, pInfo->pOutputPath);
			strcat_s(filepath, "int\\");
			sprintf_s(strNumber, "%u", pInfo->pAssetTable->pEntries[i].assetId);
			strcat_s(filepath, strNumber);
			strcat_s(filepath, GetCompiledAssetTypeExtension(pInfo->pAssetTable->pEntries[i].type));

			File file = FOpen(filepath, FileAccessType::READ);
			char* pAssetData = (char*)malloc(file.size);
			FRead(&file, pAssetData, file.size);
			FClose(&file);

			FWrite(&packageFile, pAssetData, file.size);
			free(pAssetData);
		}

		FClose(&packageFile);

		return PR_SUCCESS;
	}
}