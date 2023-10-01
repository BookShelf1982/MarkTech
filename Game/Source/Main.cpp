#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

#include <Engine.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	CEngine* engine = new CEngine();
	engine->InitEngine(hInstance, pCmdLine, nCmdShow);
	engine->StartEngineLoop();

	return 0;
}