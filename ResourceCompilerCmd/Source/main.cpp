#include <iostream>
#include <Windows.h>

typedef int (*pfnImportTexture)(const char*, const char*, int, int); // const char* filepath, int mips, int compression

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

		pfnLoadTex(argv[2], argv[3], mips, compression);
	}

	return 0;
}