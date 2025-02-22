#include <stdio.h>
#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <FileSystem.h>
#include "djb2.h"

static const char message[] = { 
	"Commands\ncompile: <assetpath>, <outputpath> = Compiles an asset.\n" 
};

#define ARG_COMPILE 0xD365602E // hash for the string "compile" 3546636334

int CompileAsset(const char* filepath, const char* outdir)
{
	if (!FSPathExists((Path*)filepath))
		return 1;

	int width, height, channel = 0;
	uint8_t* imageData = stbi_load(filepath, &width, &height, &channel, 0);

	Path outputPath;
	strcpy_s(outputPath.path, sizeof(outputPath), outdir);
	FSAppend(&outputPath, FSFindFilename((Path*)filepath));
	FSRenameExtension(&outputPath, ".tex");
	
	File file;
	FSOpen(&outputPath, OPEN_TYPE_WRITE, &file);
	if (!file._handle)
	{
		stbi_image_free(imageData);
		return 1;
	}

	FSWrite(&file, (char*)&width, sizeof(width));
	FSWrite(&file, (char*)&height, sizeof(height));
	FSWrite(&file, (char*)&channel, sizeof(channel));
	FSWrite(&file, (char*)imageData, width * height * channel);
	FSClose(&file);
	return 0;
}

int main(const int argc, const char* argv[])
{
	if (argc < 2) // if there are no arguments then print the list of available commands
	{
		printf(message);
		return 0;
	}

	uint32_t arg = djb2(argv[1]); // hash first argument to compare with an O(1)
	switch (arg)
	{
	case ARG_COMPILE:
	{
		// Compile resource
		if (CompileAsset(argv[2], argv[3]) == 1)
		{
			printf("Failed to compile: \"%s\"", argv[2]);
			return 1;
		}

		printf("Succeeded to compile: \"%s\"", argv[2]);
		return 0;
	}
	}

	return 0;
}