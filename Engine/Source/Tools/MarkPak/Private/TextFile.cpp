#include "TextFile.h"
#include <File.h>

namespace MarkTech
{
	void CompileTextFile(char* pOutputFilepath, const char* pInputFilepath)
	{
		ChangeExtension(pOutputFilepath, ".txt");

		// Open text file asset
		File textFile = FOpen(pInputFilepath, FileAccessType::READ);
		char* pTextFileContents = (char*)malloc(textFile.size);
		FRead(&textFile, pTextFileContents, textFile.size);
		FClose(&textFile);

		// Write to compiled asset
		File compiled = FOpen(pOutputFilepath, FileAccessType::WRITE);
		FWrite(&compiled, pTextFileContents, textFile.size);
		FClose(&compiled);
		free(pTextFileContents);
	}
}
