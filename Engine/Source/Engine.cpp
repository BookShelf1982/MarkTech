#include "Engine.h"

CEngine::CEngine()
{
	bClosing = false;
	Window = new CWinWindow();
	Renderer = new CD3DRenderer();
	Map = new CMap();
}

CEngine::~CEngine()
{
	delete Window;
	delete Map;
}

void CEngine::InitEngine(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow)
{
	Window->CreateWinWindow(L"WinWindow", L"MarkTech", CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, hInstance, nCmdShow);
	Renderer->InitRenderer(Window->GetHWND());

	CPointEntity* entity = new CPointEntity();
	entity->Name = "";

	Map->SpawnEntity(entity, MTransform());

	QueryPerformanceCounter(&nLastTick);
	QueryPerformanceFrequency(&nTickFrequency);
}

void CEngine::StartEngineLoop()
{
	while (!bClosing)
	{
		QueryPerformanceCounter(&nCurrentTick);
		nElapsedTicks = nCurrentTick.QuadPart - nLastTick.QuadPart;

		flDeltaTime = (float)nElapsedTicks / (float)nTickFrequency.QuadPart;

		nLastTick = nCurrentTick;

		MSG msg = {};
		while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			Window->MessageLoop(msg);
			if (msg.message == WM_QUIT)
			{
				bClosing = true;
			}
		}
		if (bClosing)
		{
			break;
		}

#ifdef DEBUGMessages
		wchar_t buffer[256];
		swprintf(buffer, 256, L"delta: %f\n", flDeltaTime);
		OutputDebugString(buffer);
#endif // DEBUGMessages

		Map->UpdateEntities(flDeltaTime);

		Renderer->RenderFrame(Window->GetHWND());

	}
}