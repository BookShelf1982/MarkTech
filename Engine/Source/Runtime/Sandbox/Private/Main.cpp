#include <Engine.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <crtdbg.h>

using namespace MarkTech;

int main()
{
	{
		Engine engine;
		engine.Init();
		engine.Shutdown();
	}

	_CrtDumpMemoryLeaks();

	return 0;
}