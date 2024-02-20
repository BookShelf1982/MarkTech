#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crtdbg.h>

#include "ModelImporter.h"
#include "ShaderImporter.h"

int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		printf("You must execute this with command line arguments!\nPress enter to exit...\n");
		return 2;
	}

	if ((strcmp("-mfx", argv[1]) == 0))
	{
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