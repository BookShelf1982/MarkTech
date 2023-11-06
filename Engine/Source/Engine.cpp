#include "Engine.h"

namespace MarkTech
{
	CEngine* CEngine::g_pEngine = new CEngine();

	CEngine::CEngine()
	{
		bClosing = false;
		Window = new CWinWindow();
	}

	CEngine::~CEngine()
	{
		DestroyWindow(Window->GetHWND());
		delete Window;
		Renderer::CD3D11Renderer::GetD3DRenderer()->DestroyRenderer();
		Configs::MUserSettings::GetUserSettings()->Destroy();
		Configs::MGameInfo::GetGameInfo()->Destroy();
		Level::CLevel::GetLevel()->DestroyLevel();
	}

	bool CEngine::InitEngine(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow)
	{
		//Read values from GameInfo.ini file
		if (!ReadConfigFiles())
		{
			return false;
		}

		//Create Window
		Window->CreateWinWindow(
			L"WinWindow",								//Class Name
			L"jknds",									//Window Title
			CW_USEDEFAULT,								//X Pos
			CW_USEDEFAULT,								//Y Pos
			800,										//Configured Width
			600,										//Configured Length
			hInstance,									//hImstance
			nCmdShow);									//nCmdShow

		Level::CLevel::GetLevel()->InitLevel();			//Initialize Level

		if (!Renderer::CD3D11Renderer::GetD3DRenderer()->InitRenderer(Window->GetHWND()))
		{
			return false;
		}

		QueryPerformanceCounter(&nLastTick);
		QueryPerformanceFrequency(&nTickFrequency);

		return true;
	}

	bool CEngine::ReadConfigFiles()
	{
		//Read alues from GameInfo.ini file
		if (!Configs::MGameInfo::GetGameInfo()->Init())
		{
			Window->CreateErrorBox(L"Unable to find GameInfo.ini");
			return false;
		}

		if (!Configs::MUserSettings::GetUserSettings()->Init())
		{
			Window->CreateErrorBox(L"Unable to find UserSettings.ini");
			return false;
		}
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
				switch (msg.message)
				{
				case WM_QUIT:
				{
					bClosing = true;
				}break;

				case WM_SYSKEYDOWN:
				case WM_SYSKEYUP:
				case WM_KEYDOWN:
				case WM_KEYUP:
				{
					uint32_t keycode = (uint32_t)msg.wParam;

					bool bWasDown = (msg.lParam & BIT(30)) != 0;
					bool bIsDown = (msg.lParam & BIT(31)) == 0;

					CInput::GetInput()->PollInput(keycode, bIsDown, bWasDown);
				}break;
				}
				if (bClosing)
				{
					break;
				}
			}

			Renderer::CD3D11Renderer::GetD3DRenderer()->RenderFrame(Window->GetHWND());

			if (CInput::GetInput()->IsKeyDown(MTVK_Escape))
			{
				Quit();
			}
		}
	}

	void CEngine::DestroyEngine()
	{
		delete g_pEngine;
	}

	void CEngine::Quit()
	{
		bClosing = true;
	}
}

int LaunchEngine(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow)
{
	if (!MarkTech::CEngine::GetEngine()->InitEngine(hInstance, pCmdLine, nCmdShow))
	{
		return 1;
	}
	MarkTech::CEngine::GetEngine()->StartEngineLoop();
	MarkTech::CEngine::GetEngine()->DestroyEngine();
	return 0;
}