#pragma once
#include <AssetTypes.h>

namespace MarkTech
{
	AssetType EvaluateAssetType(const char* pPath);
	AssetType EvaluateCompiledAssetType(const char* pPath);
}