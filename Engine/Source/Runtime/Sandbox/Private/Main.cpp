#include <Engine.h>
#include <crtdbg.h>
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

using namespace MarkTech;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	{
		Engine engine;
		engine.Init();
		engine.StartEngineLoop();
		engine.Shutdown();
	}

#ifdef DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}