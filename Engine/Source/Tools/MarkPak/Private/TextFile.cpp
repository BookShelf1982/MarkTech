#include "TextFile.h"
#include <File.h>
#include <MemoryArena.h>

namespace MarkTech
{
    TextFile ReadTextFile(const char* filepath)
    {
        TextFile fileContents = {};

        File file = FOpen(filepath, FileAccessType::READ);
        if (!file.isOpened)
            return fileContents;

        U64 bufferSize = file.size;
        fileContents.length = bufferSize;
        fileContents.pBuffer = (char*)AllocFromMemoryArena(bufferSize);

        FRead(&file, fileContents.pBuffer, bufferSize);

        FClose(&file);

        return fileContents;
    }
}