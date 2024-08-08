#pragma once
#include <PrimitiveTypes.h>

namespace MarkTech
{
	enum class AssetType
	{
		ANSI_TEXT_FILE,
		TEXTURE2D
	};

	AssetType EvaluateAssetType(const char* pPath);
}