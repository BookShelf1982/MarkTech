#include "AssetTable.h"
#include <File.h>

#include <stdlib.h>
#include <string.h>
#include <random>

namespace MarkTech
{
	U32 GetAssetListCount(const char* pAssetList)
	{
		U64 length = strlen(pAssetList);
		char* pList = (char*)malloc(length + 1);
		strcpy_s(pList, length + 1, pAssetList);
		
		char* pToken = nullptr;
		char* pNextToken = pList;
		char delimiters[] = "\r\n\0";

		U64 count = 0;
		while (pNextToken[0] != '\0')
		{
			pToken = strtok_s(pNextToken, delimiters, &pNextToken); // Get filepath to asset
			count++;
		}
		free(pList);
		return count;
	}

	AssetTable CreateAssetTableFromAssetList(const char* pFilepath)
	{
		AssetTable table = {};
		File assetListFile = FOpen(pFilepath, FileAccessType::READ);
		if (!assetListFile.isOpened)
			return table;
		
		// Read file contents
		char* pAssetTableContents = (char*)malloc(assetListFile.size + 1);
		FRead(&assetListFile, pAssetTableContents, assetListFile.size);
		FClose(&assetListFile);
		pAssetTableContents[assetListFile.size] = '\0';

		// Get the amount of assets in the asset list
		U32 listCount = GetAssetListCount(pAssetTableContents);
		table.entryCount = listCount;
		table.pEntries = (AssetTableEntry*)malloc(sizeof(AssetTableEntry) * listCount);

		// Get all asset filepaths
		char* pToken = nullptr;
		char* pNextToken = pAssetTableContents;
		char delimiters[] = "\r\n\0";

		U32 index = 0;
		while (pNextToken[0] != '\0')
		{
			pToken = strtok_s(pNextToken, delimiters, &pNextToken); // Get filepath to asset
			strcpy_s(table.pEntries[index].assetFilepath, pToken);
			{
				std::random_device rd;
				std::uniform_int_distribution<U32> dist;
				table.pEntries[index].assetId = dist(rd); // Generate random number
			}
			index++;
		}
		free(pAssetTableContents);
		return table;
	}

	void FreeAssetTable(AssetTable* pTable)
	{
		free(pTable->pEntries);
	}
}