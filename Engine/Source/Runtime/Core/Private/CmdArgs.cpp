#include "CmdArgs.h"
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace MarkTech
{
#ifdef MT_PLATFORM_WINDOWS
    CommandLineArgs CreateCommandLineArgs()
    {
        CommandLineArgs cmdArgs = {};
        cmdArgs.pArgList = CommandLineToArgvW(GetCommandLine(), &cmdArgs.argCount);
        return cmdArgs;
    }

    void FreeCommandLineArgs(CommandLineArgs* pCmdArgs)
    {
        LocalFree(pCmdArgs->pArgList);
    }
#endif
    bool ArgCompare(const CommandLineArgs* pCmdArgs, U32 index, const wchar_t* pStr)
    {
        if (pCmdArgs->pArgList[index])
            return (wcscmp(pCmdArgs->pArgList[index], pStr) == 0);

        return false;
    }
}