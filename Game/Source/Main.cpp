#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

#include <Windows.h>
#include <Engine.h>

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow )
{
	CEngine* g_pEngine = new CEngine();
	CEngine::SetEnginePtr(g_pEngine);
	if (!g_pEngine->InitEngine())
		return 1;

	g_pEngine->StartEngineLoop();

	g_pEngine->DestroyEngine();

	delete g_pEngine;

#ifdef DEBUG
	_CrtDumpMemoryLeaks();
#endif // DEBUG

	return 0;
}