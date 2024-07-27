#include "AssetTable.h"
#include <File.h>
#include <MemoryArena.h>
#include <Assert.h>
#include <string.h>

namespace MarkTech
{
    AssetTable CreateAssetTable(const char* pPath)
    {
        AssetTable table = {};

        // Read the asset table file
        File assetTableFile = FOpen(pPath, FileAccessType::READ);
        char* pFileBuffer = (char*)AllocFromMemoryArena(assetTableFile.size + 1);
        FRead(&assetTableFile, pFileBuffer, assetTableFile.size);
        pFileBuffer[assetTableFile.size] = '\0';
        FClose(&assetTableFile);
        
        // Get line numbers
        char* pPointer = pFileBuffer;
        for (U64 i = 0; i < assetTableFile.size + 1; i++)
        {
            if (pPointer[i] == '\r' && pPointer[i + 1] == '\n')
            {
                table.entryCount++;
                i++;
            }

            if (pPointer[i] == '\0')
                table.entryCount++;
        }

        // Allocate asset tabel entries
        table.pEntries = (AssetTableEntry*)AllocFromMemoryArena(sizeof(AssetTableEntry) * table.entryCount);

        // Parse tokens
        const char delimiters[] = " =\r\n";
        char* pNextToken = pFileBuffer;
        char* pToken = nullptr;
        U64 index = 0;
        while (pNextToken[0] != '\0')
        {
            // Get id
            pToken = strtok_s(pNextToken, delimiters, &pNextToken);
            U64 num = 0;
            for (int i = 0; pToken[i] != '\0'; i++) // Convert string number to uint64 number
            {
                num = num * 10 + (pToken[i] - 48);
            }

            table.pEntries[index].id = num;

            // Get path
            pToken = strtok_s(pNextToken, delimiters, &pNextToken);
            
            table.pEntries[index].pPath = pToken;

            index++;
        }

        return table;
    }

    struct ExtensionTypeUnion
    {
        char extension[7];
        AssetType type;
    };

    static ExtensionTypeUnion extensionTypeRelations[] = {
        {".txt", AssetType::ANSI_TEXT_FILE},
        {".png", AssetType::TEXTURE2D},
        {".jpg", AssetType::TEXTURE2D},
    };

    static U64 size = sizeof(extensionTypeRelations) / sizeof(ExtensionTypeUnion);

    AssetType EvaluateAssetType(char* pPath)
    {
        char* extension = GetExtension(pPath);
        for (U64 i = 0; i < size; i++)
        {
            if (strcmp(extension, extensionTypeRelations[i].extension) == 0)
            {
                return extensionTypeRelations[i].type;
            }
        }

        return AssetType::ANSI_TEXT_FILE;
    }
}