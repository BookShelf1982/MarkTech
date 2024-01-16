#include "Engine.h"

CEngine::CEngine()
	:m_bClosing(false)
{
	m_hInstance = NULL;
	m_pWindow = new CWinWindow();
	m_pLevel = new CLevel();
	m_pInput = new CInput();
	m_pAssetRegistry = new CAssetRegistry();
	m_pRenderer = nullptr;
}

CEngine::~CEngine()
{
	delete m_pWindow;
	delete m_pLevel;
	delete m_pInput;
	delete m_pRenderer;
	delete m_pAssetRegistry;
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

	uint64_t v1 = m_pLevel->CreateVessel();
	MModelComponent comp = m_pLevel->CreateComponent<MModelComponent>();
	m_pLevel->AttachComponentToVessel(v1, comp);
	m_pLevel->DestroyVessel(v1);

	if (!CreateDX11Renderer(&m_pRenderer))
		return false;

	m_pRenderer->SetWindow(m_pWindow);

	if (!m_pRenderer->InitRenderer())
		return false;

	return true;
}

void CEngine::DestroyEngine()
{
	m_pWindow->KillWindow();
	m_pLevel->DestroyLevel();
	m_pInput->DestroyInput();
	m_pAssetRegistry->DestroyAssetRegistry();
	m_pRenderer->ShutdownRenderer();
}

void CEngine::StartEngineLoop()
{
	m_RenderThread = std::thread(&CEngine::RenderLoop, this);
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
			if (m_bClosing)
			{
				break;
			}
		}
		if (m_bClosing)
		{
			break;
		}
		if (m_pInput->IsKeyDown(MTKI_ESCAPE))
		{
			Close();
		}
		m_pLevel->UpdateLevel(0);
	}
	m_RenderThread.join();
}

void CEngine::RenderLoop()
{
	while (!m_bClosing)
	{
		m_pRenderer->RenderFrame();
	}
}

void CEngine::Close()
{
	m_bClosing = true;
}