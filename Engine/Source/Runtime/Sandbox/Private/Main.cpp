#include <Memory\StackAllocator.h>
#include <stdio.h>
#include <crtdbg.h>

using namespace MarkTech;

int main()
{
	{
		StackAllocator allocator(1024, 4);
		StackMarker marker = allocator.Mark();
		for (U32 i = 0; i < 12; i++)
		{
			marker = allocator.Mark();
			U32* pNum = (U32*)allocator.Alloc(sizeof(U32));
			*pNum = 300;
			printf("%u\n", *pNum);

			if (i==11)
				printf("button");
		}
		allocator.FreeToMark(marker);
	}

	_CrtDumpMemoryLeaks();

	return 0;
}