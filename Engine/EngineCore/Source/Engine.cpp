#include "Engine.h"

CEngine::CEngine()
	:m_bClosing(false)
{
	m_hInstance = NULL;
	m_pWindow = new CWinWindow();
	m_pLevel = new CLevel();
	m_pInput = new CInput();
	m_pAssetRegistry = new CAssetRegistry();
	m_pRenderer = new C3DRenderer();
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
	MTransformComponent comp2 = m_pLevel->CreateComponent<MTransformComponent>();
	m_pLevel->AttachComponentToVessel(v1, comp);
	m_pLevel->AttachComponentToVessel(v1, comp2);

	uint64_t v2 = m_pLevel->CreateVessel();
	MModelComponent comp20 = m_pLevel->CreateComponent<MModelComponent>();
	MTransformComponent comp22 = m_pLevel->CreateComponent<MTransformComponent>();
	m_pLevel->AttachComponentToVessel(v2, comp20);
	m_pLevel->AttachComponentToVessel(v2, comp22);
	//m_pLevel->DestroyVessel(v1);

	m_pRenderer->SetRenderAPI(ERendererAPI::Vulkan);

	if (!m_pRenderer->Init(m_pWindow, m_pAssetRegistry))
		return false;

	return true;
}

void CEngine::DestroyEngine()
{
	m_pWindow->KillWindow();
	m_pLevel->DestroyLevel();
	m_pInput->DestroyInput();
	m_pAssetRegistry->DestroyAssetRegistry();
	m_pRenderer->Destroy();
}

void CEngine::StartEngineLoop()
{
	m_GameThread = std::thread(&CEngine::GameLoop, this);
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

		m_pRenderer->RenderFrame();
	}
	m_GameThread.join();
}

void CEngine::GameLoop()
{
	while (!m_bClosing)
	{
		if (m_pInput->IsKeyDown(MTKI_ESCAPE))
		{
			Close();
		}
		m_pLevel->UpdateLevel(0);
		//m_pLevel->GiveSceneBuffer(m_pRenderer);
	}
}

void CEngine::Close()
{
	m_bClosing = true;
}