#pragma once
#include <PrimitiveTypes.h>
#include "AssetTable.h"

namespace MarkTech
{
	enum CompilerResult
	{
		CR_SUCCESS = 0x00,
		CR_UNKNOWN_ASSET_TYPE = 0x01,
		CR_FILE_IO_FAILED = 0x02
	};

	CompilerResult CompileAsset(AssetTable* pTable, const char* pOutputPath);
}