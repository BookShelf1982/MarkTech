#pragma once
#include <AssetTypes.h>

namespace MarkTech
{
	char* GetAssetTypeExtension(AssetType type);
	char* GetCompiledAssetTypeExtension(AssetType type);
	AssetType EvaluateAssetType(const char* pPath);
	AssetType EvaluateCompiledAssetType(const char* pPath);
}
