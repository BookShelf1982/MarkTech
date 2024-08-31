#include <File.h>
#include <AssetTypes.h>
#include <AssetCompiler.h>
#include <Package.h>

#ifdef DEBUG
#include <crtdbg.h>
#endif

using namespace MarkTech;


int main(int argc, char* argv[])
{
	// Parse command line args
	char* pInputPath = nullptr;
	char* pOutputPath = nullptr;
	char* pPackageName = nullptr;

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-input") == 0)
		{
			pInputPath = argv[i + 1];
			i++;
			continue;
		}

		if (strcmp(argv[i], "-output") == 0)
		{
			pOutputPath = argv[i + 1];
			i++;
			continue;
		}

		if (strcmp(argv[i], "-name") == 0)
		{
			pPackageName = argv[i + 1];
			i++;
			continue;
		}
	}

	if (!pInputPath || !pOutputPath || !pPackageName)
		return 1;

	// Initialization
	LinkFileSystem();

	// Read asset table
	File assetTableFile = FOpen(pInputPath, FileAccessType::READ);
	char* pAssetTableContents = (char*)malloc(assetTableFile.size + 1);
	FRead(&assetTableFile, pAssetTableContents, assetTableFile.size);
	FClose(&assetTableFile);
	pAssetTableContents[assetTableFile.size] = '\0';

	// Compile all assets
	char* pToken = nullptr;
	char* pNextToken = pAssetTableContents;
	char delimiters[] = "\r\n\0";

	while (pNextToken[0] != '\0')
	{
		pToken = strtok_s(pNextToken, delimiters, &pNextToken); // Get filepath to asset

		// Compile Asset
		CompileAsset(pToken, pOutputPath);
	}

	free(pAssetTableContents); // Free the asset table

	// Package all compiled assets
	PackageCompiledAssets(pOutputPath, pPackageName);

	// Destruction
	UnlinkFileSystem();

#ifdef DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}