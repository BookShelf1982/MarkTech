#include <stdio.h>
#include <DynArray.h>
#include <LinkedList.h>
#include <crtdbg.h>

using namespace MarkTech;

int main()
{
	{
		LinkedList<U32> list;
		list.Insert(29);
		list.Insert(20);
		list.Insert(245);
		list.Insert(242);
	}

	_CrtDumpMemoryLeaks();

	return 0;
}