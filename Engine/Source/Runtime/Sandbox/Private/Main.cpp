#include <stdio.h>
#include <DynArray.h>
#include <LinkedList.h>
#include <File.h>
#include <crtdbg.h>

using namespace MarkTech;

int main()
{
	{
		LinkedList<U32> list;
		list.Insert(29);
		list.Insert(20);
		list.Remove(1);
		list.Insert(245);
		list.Insert(242);

		for (LinkedList<U32>::Iterator i = list.Begin(); i != nullptr; i++)
		{
			printf("%u\n", i->data);
		}

		File file("wee.dat", FileAccessType::WRITE);
		
		U32 number = 123456789;

		file.Write((char*)&number, sizeof(U32));
		file.Close();
		printf("Write file: %u", number);

		File writeFile("wee.dat", FileAccessType::READ);

		U32 newNumber = 0;

		writeFile.Read((char*)&newNumber, sizeof(U32));
		writeFile.Close();
		printf("Read file: %u", newNumber);
	}

	_CrtDumpMemoryLeaks();

	return 0;
}