#include "AssetTypes.h"
#include <Path.h>
#include <string.h>

namespace MarkTech
{
    struct ExtensionTypeUnion
    {
        char extension[7];
        AssetType assetType;
    };

    static ExtensionTypeUnion compiledExtensionTypes[] = {
        {".txt", AssetType::ANSI_TEXT_FILE},
        {".tex", AssetType::TEXTURE2D},
        {".spv", AssetType::SHADER},
        {".mfx", AssetType::EFFECTS}
    };

    static U64 compiledExtensionTypesSize = sizeof(compiledExtensionTypes) / sizeof(ExtensionTypeUnion);

    static ExtensionTypeUnion extensionTypeRelations[] = {
        {".txt", AssetType::ANSI_TEXT_FILE},
        {".png", AssetType::TEXTURE2D},
        {".jpg", AssetType::TEXTURE2D},
        {".mfx", AssetType::EFFECTS},
        {".spv", AssetType::SHADER},
    };

    static U64 extensionTypeRelationsSize = sizeof(extensionTypeRelations) / sizeof(ExtensionTypeUnion);

    char* GetAssetTypeExtension(AssetType type)
    {
        for (U64 i = 0; i < extensionTypeRelationsSize; i++)
        {
            if (type == extensionTypeRelations[i].assetType)
                return extensionTypeRelations[i].extension;
        }
    }

    char* GetCompiledAssetTypeExtension(AssetType type)
    {
        for (U64 i = 0; i < compiledExtensionTypesSize; i++)
        {
            if (type == compiledExtensionTypes[i].assetType)
                return compiledExtensionTypes[i].extension;
        }
    }

    AssetType EvaluateAssetType(const char* pPath)
    {
        char* extension = GetExtension(pPath);
        for (U64 i = 0; i < extensionTypeRelationsSize; i++)
        {
            if (strcmp(extension, extensionTypeRelations[i].extension) == 0)
            {
                return extensionTypeRelations[i].assetType;
            }
        }

        return AssetType::ANSI_TEXT_FILE;
    }

    AssetType EvaluateCompiledAssetType(const char* pPath)
    {
        char* extension = GetExtension(pPath);
        for (U64 i = 0; i < compiledExtensionTypesSize; i++)
        {
            if (strcmp(extension, compiledExtensionTypes[i].extension) == 0)
            {
                return compiledExtensionTypes[i].assetType;
            }
        }

        return AssetType::ANSI_TEXT_FILE;
    }
}