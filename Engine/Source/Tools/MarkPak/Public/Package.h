#pragma once
#include "AssetTable.h"

namespace MarkTech
{
#define MT_SIGNATURE_LENGTH 32

	enum PackageResult
	{
		PR_SUCCESS = 0x00,
		PR_FILE_ERROR = 0x01
	};

	enum PackageBitFlags
	{
		PACKAGE_FLAGS_STRINGTABLE = 0x01
	};

	struct PackageMetadata
	{
		//U64 assetCount;
		char signature[MT_SIGNATURE_LENGTH];
	};

	struct PackageCompiledAssetsCreateInfo
	{
		const AssetTable* pAssetTable;
		const char* pOutputPath;
		const char* pPackageName;
		PackageMetadata packageMetadata;
		U32 packageFlags;
	};

	PackageResult PackageCompiledAssets(const PackageCompiledAssetsCreateInfo* pInfo);
}