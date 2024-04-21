#include <Windows.h>
#include "SandboxApp.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	{
		CSandboxApp app;
		app.Run();
	}

#ifdef DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}