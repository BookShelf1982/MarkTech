#include <Memory\PoolAllocator.h>
#include <DSA\DynArray.h>
#include <stdio.h>
#include <memory.h>
#include <crtdbg.h>

using namespace MarkTech;

int main()
{
	{
		printf("Hello World!");
	}

	_CrtDumpMemoryLeaks();

	return 0;
}