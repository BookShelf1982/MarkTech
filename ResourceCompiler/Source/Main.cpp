#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crtdbg.h>

#include "TextureImporter.h"
#include "ModelImporter.h"
#include "ShaderImporter.h"

int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		printf("You must execute this with command line arguments!\nPress enter to exit...\n");
		return 2;
	}

	if ((strcmp("-mtex", argv[1]) == 0))
	{
		if ((strcmp("-mips", argv[4]) != 0))
		{
			printf("Couldn't find -mips argument!\n");
			return 1;
		}
		char* tempChar;
		int mips = strtol(argv[5], &tempChar, 10);
		if ((strcmp("-compression", argv[6]) != 0))
		{
			printf("Couldn't find -compression argument!\n");
			return 1;
		}
		int compression = -1;
		if ((strcmp("NONE", argv[7]) == 0))
			compression = -1;
		else if ((strcmp("DXT1", argv[7]) == 0))
			compression = 0;
		else if ((strcmp("DXT5", argv[7]) == 0))
			compression = 1;

		bool srgb = false;
		if (argv[8] != nullptr)
		{
			if ((strcmp("-srgb", argv[8]) == 0))
				srgb = true;
		}
		LoadTexture(argv[2], argv[3], mips, compression, srgb);
		return 0;
	}

	if ((strcmp("-mfx", argv[1]) == 0))
	{
		/*if ((strcmp("-entrypoint", argv[4]) != 0))
		{
			printf("Couldn't find -entrypoint argument!\n");
			return 1;
		}*/

		LoadShader(argv[2], argv[3], "main");
		return 0;
	}

	if ((strcmp("-mmdl", argv[1]) == 0))
	{
		if (argv[2] == nullptr)
		{
			printf("Please intput an input file path argument!");
			return 1;
		}
		if (argv[3] == nullptr)
		{
			printf("Please input an output file path argument!");
			return 1;
		}

		LoadModel(argv[2], argv[3]);
		return 0;
	}

#ifdef DEBUG
	_CrtDumpMemoryLeaks();
#endif // DEBUG
	return 0;
}