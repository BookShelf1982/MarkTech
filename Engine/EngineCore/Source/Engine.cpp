#include "Engine.h"

CEngine::CEngine()
{
	m_pWindow = new CWinWindow();
	m_pLevel = new CLevel();
	m_pInput = new CInput();
}

CEngine::~CEngine()
{
	delete m_pWindow;
	delete m_pLevel;
	delete m_pInput;
}

void CEngine::PreInitEngine(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
}

bool CEngine::InitEngine(pfnCloseGame pfn)
{
	m_pWindow->SetHInstance(m_hInstance);
	m_pWindow->MakeWindow("Marktech", 0, 0, 640, 480, EWindowed);
	if (!m_pInput->InitInput(m_hInstance, m_pWindow->GetHWND()))
		return false;
	m_pLevel->InitLevel();
	CloseGame = pfn;
	return true;
}

void CEngine::DestroyEngine()
{
	m_pWindow->KillWindow();
	m_pLevel->DestroyLevel();
	m_pInput->DestroyInput();
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
	m_pInput->PollInput();
	m_pLevel->UpdateLevel(0);
	if (m_pInput->IsKeyDown(DIK_ESCAPE))
	{
		CloseGame();
	}
}
