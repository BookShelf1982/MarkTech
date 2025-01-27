#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <StringId\StringId.h>

using namespace MarkTech;

uint32_t hash(const char* str)
{
	uint32_t hash = 5381;
	int32_t c = 0;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

int main(int argc, const char* argv[])
{
	StringId endCode = "end123"_sid;

	/*AssetEntryInfo info = {};

	for (uint32_t i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-n") == 0)
		{
			info.name = (char*)argv[i + 1];
			i++;
		}
		else if(strcmp(argv[i], "-p") == 0)
		{
			info.path = (char*)argv[i + 1];
			i++;
		}
		else if (strcmp(argv[i], "-o") == 0)
		{
			info.outputPath = (char*)argv[i + 1];
			i++;
		}
		else if (strcmp(argv[i], "-t") == 0)
		{
			info.type = (AssetType)atoi(argv[i + 1]);
			i++;
		}
	}

	info.id = 69696088;
	WriteAssetEntryFile(&info);
	*/

	while (true)
	{
		printf("Hash string\n");
		char input[128] = "";
		fgets(input, sizeof(input), stdin);
		size_t len = strlen(input);
		input[len - 1] = 0;
		uint32_t hashInput = hash(input);
		printf("%s hashed: %u\n", input, hashInput);
		if (hashInput == endCode)
			break;
	}

	return 0;
}