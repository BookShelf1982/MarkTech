#include "Engine.h"

namespace MarkTech
{
	CEngine* CEngine::g_pEngine = new CEngine();

	CEngine::CEngine()
	{
		bClosing = false;
		m_pMainWindow = new CWinWindow();
		m_pLevel = new CLevel();
		m_pGameInfo = new MGameInfo();
		m_pUserSettings = new MUserSettings();
		m_pInput = new CInput();
	}

	CEngine::~CEngine()
	{
		delete m_pGameInfo;
		delete m_pUserSettings;
		m_pLevel->DestroyLevel();
		m_pInput->DestroyInput();
		delete m_pMainWindow;
	}

	//Engine init func
	bool CEngine::InitEngine(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow)
	{
		//Read values from GameInfo.ini file
		if (!ReadConfigFiles())
		{
			return false;
		}

		//Create Window
		m_pMainWindow->CreateWinWindow(
			L"WinWindow",									//Class Name
			L"jknds",										//Window Title
			CW_USEDEFAULT,									//X Pos
			CW_USEDEFAULT,									//Y Pos
			GetUserSettings().,						//Configured Width
			GetUserSettings()->nVSHeight,						//Configured Length
			hInstance,										//hImstance
			nCmdShow);										//nCmdShow

		if (!m_pLevel->InitLevel())	//Initialize Level
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
		if (!m_pGameInfo->Init())
		{
			m_pMainWindow->CreateErrorBox(L"Unable to find GameInfo.ini");
			return false;
		}

		if (!m_pUserSettings->Init(*m_pGameInfo))
		{
			m_pMainWindow->CreateErrorBox(L"Unable to find or generate UserSettings.ini");
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
				m_pMainWindow->MessageLoop(msg);
				switch (msg.message)
				{
				case WM_QUIT:
				{
					bClosing = true;
				}break;
				}
				if (bClosing)
				{
					break;
				}
			}

			m_pInput->PollInput();

			if (m_pInput->IsKeyDown(MTIK_ESCAPE))
			{
				Quit();
			}

			m_pLevel->UpdateLevel(flDeltaTime);
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
#ifdef DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}

int LaunchEditor(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow)
{
	if (!MarkTech::CEngine::GetEngine()->InitEditor(hInstance, pCmdLine, nCmdShow))
	{
		return 1;
	}
	MarkTech::CEngine::GetEngine()->StartEngineLoop();
	MarkTech::CEngine::GetEngine()->DestroyEngine();
	return 0;
}
