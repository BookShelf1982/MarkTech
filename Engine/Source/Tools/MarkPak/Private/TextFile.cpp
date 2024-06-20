#include "TextFile.h"
#include <Memory\MemoryManager.h>
#include <File.h>

namespace MarkTech
{
    TextFile ReadTextFile(const char* filepath)
    {
        TextFile fileContents = {};

        File file(filepath, FileAccessType::READ);
        if (!file.IsOpen())
            return fileContents;

        U64 bufferSize = file.GetSize();
        fileContents.length = bufferSize;
        fileContents.pBuffer = (char*)MemoryManager::Alloc(bufferSize, 1);

        file.Read(fileContents.pBuffer, bufferSize);
        file.Close();

        return fileContents;
    }
}