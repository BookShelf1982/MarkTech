#include "Engine.h"

CEngine::CEngine()
{
	m_pWindow = new CWinWindow();
	m_pLevel = new CLevel();
}

CEngine::~CEngine()
{
	delete m_pWindow;
	delete m_pLevel;
}

bool CEngine::InitEngine(pfnCloseGame pfn)
{
	m_pWindow->MakeWindow("Marktech", 0, 0, 640, 480, EWindowed);
	m_pLevel->InitLevel();
	CloseGame = pfn;
	return true;
}

void CEngine::DestroyEngine()
{
	m_pWindow->KillWindow();
	m_pLevel->DestroyLevel();
}

void CEngine::UpdateEngine()
{
	MSG msg = {};
	while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
		switch (msg.message)
		{
		case WM_QUIT:
		{
			CloseGame();
		}break;
		}
	}

	m_pLevel->UpdateLevel(0);
}
