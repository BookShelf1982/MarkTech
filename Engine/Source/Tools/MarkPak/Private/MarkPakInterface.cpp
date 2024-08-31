#include "MarkPakInterface.h"
#include <AssetCompiler.h>
#include <Package.h>
#include <File.h>

void CompileAndPackageAssets(const char* pInputFilepath, const char* pOutputPath, const char* pPackageName)
{
	using namespace MarkTech;
	// Read asset table
	File assetTableFile = FOpen(pInputFilepath, FileAccessType::READ);
	if (!assetTableFile.isOpened)
		return;

	char* pAssetTableContents = (char*)malloc(assetTableFile.size + 1);
	FRead(&assetTableFile, pAssetTableContents, assetTableFile.size);
	FClose(&assetTableFile);
	pAssetTableContents[assetTableFile.size] = '\0';

	// Compile all assets
	char* pToken = nullptr;
	char* pNextToken = pAssetTableContents;
	char delimiters[] = "\r\n\0";

	while (pNextToken[0] != '\0')
	{
		pToken = strtok_s(pNextToken, delimiters, &pNextToken); // Get filepath to asset

		// Compile Asset
		CompileAsset(pToken, pOutputPath);
	}

	free(pAssetTableContents); // Free the asset table

	// Package all compiled assets
	PackageCompiledAssets(pOutputPath, pPackageName);
}

void GetMarkPakFunctions(MarkPakPFNList* pPfnList)
{
	pPfnList->pfnCompileAsset = MarkTech::CompileAsset;
	pPfnList->pfnPackageCompiledAssets = MarkTech::PackageCompiledAssets;
	pPfnList->pfnCompileAndPackageAssets = CompileAndPackageAssets;
}
