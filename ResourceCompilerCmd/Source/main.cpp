#include <iostream>
#include <Windows.h>

typedef int (*pfnImportTexture)(const char*, const char*, int, int, bool); // const char* filepath, const char* output, int mips, int compression, bool srgb
typedef int (*pfnImportShader)(const char*, const char*, const char*, const char*); // const char* filepath, const char* output, const char* shadertype, const char* entrypoint
typedef int (*pfnImportModel)(const char*, const char*); // const char* filepath, const char* output

int main(int argc, const char* argv[])
{
	if (argc <= 1)
	{
		std::cout << "You must execute this with command line arguments!\nPress enter to exit...\n";
		std::cin.get();
		return 0;
	}

	HMODULE ResourceCompilerLib = LoadLibraryA("ResourceCompiler.dll");
	if (ResourceCompilerLib == NULL)
	{
		std::cout << "Can't find ResourceCompiler.dll!\nPress enter to exit...\n";
		std::cin.get();
		return 0;
	}

	if ((strcmp("-mtex", argv[1]) == 0))
	{
		pfnImportTexture pfnLoadTex = (pfnImportTexture)GetProcAddress(ResourceCompilerLib, "LoadTexture");
		if ((strcmp("-mips", argv[4]) != 0))
		{
			std::cout << "Couldn't find -mips argument!\nPress enter to exit...\n";
			return -1;
		}
		char *tempChar;
		int mips = strtol(argv[5], &tempChar, 10);
		if ((strcmp("-compression", argv[6]) != 0))
		{
			std::cout << "Couldn't find -compression argument!\nPress enter to exit...\n";
			return -1;
		}
		int compression = -1;
		if ((strcmp("NONE", argv[7]) == 0))
			compression = -1;
		else if((strcmp("DXT1", argv[7]) == 0))
			compression = 0;
		else if ((strcmp("DXT5", argv[7]) == 0))
			compression = 1;

		bool srgb = false;
		if (argv[8] != nullptr)
		{
			if ((strcmp("-srgb", argv[8]) == 0))
				srgb = true;
		}
		pfnLoadTex(argv[2], argv[3], mips, compression, srgb);
	}

	if ((strcmp("-mfx", argv[1]) == 0))
	{
		pfnImportShader pfnLoadShader = (pfnImportShader)GetProcAddress(ResourceCompilerLib, "LoadShader");
		if ((strcmp("-compiler", argv[4]) != 0))
		{
			std::cout << "Couldn't find -compiler argument!\nPress enter to exit...\n";
			std::cin.get();
			return -1;
		}
		if ((strcmp("-entrypoint", argv[6]) != 0))
		{
			std::cout << "Couldn't find -entrypoint argument!\nPress enter to exit...\n";
			std::cin.get();
			return -1;
		}

		pfnLoadShader(argv[2], argv[3], argv[5], argv[7]);
	}

	if ((strcmp("-mmdl", argv[1]) == 0))
	{
		pfnImportModel pfnLoadModel = (pfnImportModel)GetProcAddress(ResourceCompilerLib, "LoadModel");
		if (argv[2] == nullptr)
		{
			std::cout << "Please intput an input file path argument!\nPress enter to exit...\n";
			std::cin.get();
			return -1;
		}
		if (argv[3] == nullptr)
		{
			std::cout << "Please input an output file path argument!\nPress enter to exit...\n";
			std::cin.get();
			return -1;
		}

		pfnLoadModel(argv[2], argv[3]);
	}
	FreeLibrary(ResourceCompilerLib);
	return 0;
}