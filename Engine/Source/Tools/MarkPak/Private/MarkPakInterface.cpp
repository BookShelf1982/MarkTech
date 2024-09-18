#include "MarkPakInterface.h"
#include "AssetTable.h"
#include "AssetCompiler.h"
#include "Package.h"

#include <string.h>
MarkPakPFNList gpfnList = {};

namespace MarkTech
{
	void CompileAndPackageAssets(const char* pFileList, const char* pOutputPath, const char* pPakName)
	{
		// Get asset table from asset list file
		AssetTable table = CreateAssetTableFromAssetList(pFileList);
		// Compile assets
		CompileAsset(&table, pOutputPath);

		// Package compiled assets
		PackageCompiledAssetsCreateInfo info = {};
		info.pOutputPath = pOutputPath;
		info.pPackageName = pPakName;
		info.packageFlags = PACKAGE_FLAGS_STRINGTABLE;
		info.pAssetTable = &table;
		strcpy_s(info.packageMetadata.signature, "MarkTech-VeggieTales-McBingus");
		PackageCompiledAssets(&info);

		// Destroy objects
		FreeAssetTable(&table);
	}
}

void GetMarkPakFunctions(MarkPakPFNList* pPfnList)
{
	pPfnList->pfnCompileAndPackageAssets = MarkTech::CompileAndPackageAssets;
	gpfnList = *pPfnList;
}