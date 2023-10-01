#include "Engine.h"

CEngine::CEngine()
{
	bClosing = false;
}

CEngine::~CEngine()
{

}

void CEngine::InitEngine(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow)
{
	Logger->Init();
	Window->CreateWinWindow(L"WinWindow", L"MarkTech", CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, hInstance, nCmdShow);
}

void CEngine::StartEngineLoop()
{
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		Window->MessageLoop(msg);
	}
}