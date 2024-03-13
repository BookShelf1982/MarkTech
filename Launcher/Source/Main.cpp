#include <Windows.h>
#include "SandboxApp.h"

#ifdef DEBUG
#include <crtdbg.h>
#endif

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	CSandboxApp app;
	app.Run();

	return 0;
}