#include "MarkPakInterface.h"
#include "AssetTable.h"
#include "AssetCompiler.h"
#include "Package.h"

#include <string.h>
MarkPakPFNList gpfnList = {};

namespace MarkTech
{
	void CompileAndPackageAssets(const char* pFileList, const char* pOutputPath, const char* pPakName, const char* pSignature, U32 flags)
	{
		// Get asset table from asset list file
		AssetTable table = CreateAssetTableFromAssetList(pFileList);
		
		// Compile assets
		CompileAsset(&table, pOutputPath);

		// Package compiled assets
		PackageCompiledAssetsCreateInfo info = {};
		info.pOutputPath = pOutputPath;
		info.pPackageName = pPakName;
		info.packageFlags = flags;
		info.pAssetTable = &table;
		strcpy_s(info.packageMetadata.signature, pSignature);
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