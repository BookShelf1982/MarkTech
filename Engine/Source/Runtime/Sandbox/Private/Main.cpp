#include <Memory\StackAllocator.h>
#include <stdio.h>
#include <memory.h>
#include <crtdbg.h>

using namespace MarkTech;

int main()
{
	{
		StackAllocator allocator(1024, 1);
		for (U32 i = 0; i < 3; i++)
		{
			allocator.Mark();
			char* pBuffer = (char*)allocator.Alloc(sizeof(char) * 14);
			char string[14] = "Hello World!\n";
			memcpy(pBuffer, string, 14);
		}

		allocator.FreeToMark();
		allocator.Clear();
	}

	_CrtDumpMemoryLeaks();

	return 0;
}