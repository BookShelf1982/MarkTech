#include <Memory\MemoryManager.h>
#include <DSA\DSManager.h>
#include <Package.h>
#include <TextFile.h>
#include <crtdbg.h>

using namespace MarkTech;

int main()
{
	// Init
	MemoryManager m_pMemManager;
	DSManager m_pDSManager;

	m_pMemManager.Init(4 * MEGABYTE, 0);
	m_pDSManager.Init(1024, 32);

	{
		// Create package
		Package pack;

		// Create text file
		TextFile textFile = ReadTextFile("text.txt");
		TextFile textFile2 = ReadTextFile("text1.txt");

		// Add text file to package
		PackageEntry textFileEntry = {};
		textFileEntry.entryId = 1;
		textFileEntry.entrySize = textFile.length;
		textFileEntry.entryType = EntryType::ANSI;
		textFileEntry.pData = textFile.pBuffer;

		PackageEntry textFileEntry2 = {};
		textFileEntry2.entryId = 2;
		textFileEntry2.entrySize = textFile2.length;
		textFileEntry2.entryType = EntryType::ANSI;
		textFileEntry2.pData = textFile2.pBuffer;

		pack.AddPackageEntry(textFileEntry);
		pack.AddPackageEntry(textFileEntry2);

		// Write package to file
		pack.WritePackageToFile("output.mpk");
	}

	// Shutdown
	m_pDSManager.Shutdown();
	m_pMemManager.Shutdown();

#ifdef DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}