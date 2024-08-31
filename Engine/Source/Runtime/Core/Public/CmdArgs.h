#pragma once
#include <PrimitiveTypes.h>

namespace MarkTech
{
	struct CommandLineArgs
	{
		wchar_t** pArgList;
		I32 argCount;
	};

	CommandLineArgs CreateCommandLineArgs();
	bool ArgCompare(const CommandLineArgs* pCmdArgs, U32 index, const wchar_t* pStr);
	void FreeCommandLineArgs(CommandLineArgs* pCmdArgs);
}