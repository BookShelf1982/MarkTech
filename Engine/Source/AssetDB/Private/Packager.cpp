#include "Packager.h"
#include <FileSystem\FileSystem.h>

#include <stdio.h>

bool PackageAllFiles(const char* rootPath, const char* oututPath)
{
	FileInfo info = {};
	FileIterator it = FSBeginFileIt(rootPath, &info);
	do
	{
		printf("%s\n", info.name);
	} while (FSFileIterate(it, &info));
	FSEndFileIt(it);


	return true;
}
