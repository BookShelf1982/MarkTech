#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

#include <Engine.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	CEngine* engine = new CEngine();

	if (!engine->InitEngine(hInstance, pCmdLine, nCmdShow))
	{
		return 0;
	}
	engine->StartEngineLoop();
	delete engine;

	return 0;
}