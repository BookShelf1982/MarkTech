#include "Engine.h"

CEngine::CEngine()
	:m_bClosing(false)
{
	m_pWindow = new CWinWindow();
	m_pLevel = new CLevel();
	m_pInput = new CInput();
	m_pRenderer = nullptr;
}

CEngine::~CEngine()
{
	delete m_pWindow;
	delete m_pLevel;
	delete m_pInput;
	delete m_pRenderer;
}

void CEngine::PreInitEngine(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
}

bool CEngine::InitEngine()
{
	m_pWindow->SetHInstance(m_hInstance);
	m_pWindow->MakeWindow("Marktech", 0, 0, 640, 480, EWindowed, m_pInput);
	if (!m_pInput->InitInput())
		return false;
	m_pLevel->InitLevel();

	if (!CreateDX11Renderer(&m_pRenderer))
		return false;

	m_pRenderer->SetWindow(m_pWindow);

	if (!m_pRenderer->InitRenderer())
		return false;

	MGenericVertex v[3] = {
		{0.0f, 0.0f, 0.0f},
		{1.0f, -1.0f, 1.0f},
		{1.0f, -1.0f, 1.0f}
	};

	IVertexBuffer* buffer = m_pRenderer->CreateVertexBuffer(v, 3);

	buffer->ReleaseBuffer();
	return true;
}

void CEngine::DestroyEngine()
{
	m_pWindow->KillWindow();
	m_pLevel->DestroyLevel();
	m_pInput->DestroyInput();
	m_pRenderer->ShutdownRenderer();
}

void CEngine::StartEngineLoop()
{
	while (!m_bClosing)
	{
		MSG msg = {};
		while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			m_pWindow->MessageLoop(&msg);
			switch (msg.message)
			{
			case WM_QUIT:
			{
				Close();
			}break;
			}
		}
		if (m_pInput->IsKeyDown(MTKI_ESCAPE))
		{
			Close();
		}
		m_pLevel->UpdateLevel(0);
		m_pRenderer->RenderFrame();
	}
}

void CEngine::Close()
{
	m_bClosing = true;
}