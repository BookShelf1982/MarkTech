#include "Database.h"
#include <stdint.h>
#include <string.h>

/*
* Commands:
*	add <asset-name>, <db-path>, <src-asset-path> = Adds an asset to the database
*	remove <asset-name> = Removes an asset from the database
*	compile <root-db-path>, <output-path> = Compiles all assets in root path
*	package <src-path>, <output-path> = Packages all assets into a mpk file
*/

int main(int argc, char* argv[])
{
	if (strcmp(argv[1], "add") == 0)
	{
		return AddAsset(argv[2], argv[4], argv[3]);
	}

	return 1;
}