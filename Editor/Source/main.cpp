#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

#include <Windows.h>
#include <Engine.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	CEngine* g_pEngine = new CEngine();
	g_pEngine->InitEngine();



#ifdef DEBUG
	_CrtDumpMemoryLeaks();
#endif // DEBUG

	return 0;
}