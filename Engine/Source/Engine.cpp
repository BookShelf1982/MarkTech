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
	delete Renderer;
	delete Map;
}

bool CEngine::InitEngine(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow)
{
	if (!MGameInfo::GetGameInfo().Init())
	{
		Window->CreateWinMessageBox();
		return false;
	}

	std::wstring WindowName(MGameInfo::GetGameInfo().GameName.begin(), MGameInfo::GetGameInfo().GameName.end());
	Window->CreateWinWindow(L"WinWindow", WindowName.c_str(), CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, hInstance, nCmdShow);
	Renderer->InitRenderer(Window->GetHWND());

	QueryPerformanceCounter(&nLastTick);
	QueryPerformanceFrequency(&nTickFrequency);

	return true;
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
