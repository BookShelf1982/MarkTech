#include "Engine.h"

namespace MarkTech
{
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
		delete m_pLevel;
		delete m_pInput;
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
			m_pUserSettings->GetWidth(),					//Configured Width
			m_pUserSettings->GetHeight(),					//Configured Length
			hInstance,										//hImstance
			nCmdShow);										//nCmdShow

		m_pInput->InitInput(hInstance, m_pMainWindow->GetHWND());

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

	void CEngine::Quit()
	{
		bClosing = true;
	}
}