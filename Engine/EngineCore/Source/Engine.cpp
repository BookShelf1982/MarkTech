#include "Engine.h"

CEngine* CEngine::m_pEngine = nullptr;

CEngine::CEngine()
	:m_EngineMemoryPool(2048, 4, 0, true)
{
	m_pWindow = nullptr;
	m_pLevel = (CLevel*)m_EngineMemoryPool.GetMemory(sizeof(CLevel));
	m_pInput = (CInput*)m_EngineMemoryPool.GetMemory(sizeof(CInput));
	m_pAssetRegistry = (CAssetRegistry*)m_EngineMemoryPool.GetMemory(sizeof(CAssetRegistry));
	m_pRenderer = (C3DRenderer*)m_EngineMemoryPool.GetMemory(sizeof(C3DRenderer));
}

CEngine::~CEngine()
{
	m_EngineMemoryPool.FreeMemory(m_pLevel, sizeof(CLevel));
	m_EngineMemoryPool.FreeMemory(m_pInput, sizeof(CInput));
	m_EngineMemoryPool.FreeMemory(m_pRenderer, sizeof(C3DRenderer));
	m_EngineMemoryPool.FreeMemory(m_pAssetRegistry, sizeof(CAssetRegistry));
}

bool CEngine::InitEngine()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_pWindow = glfwCreateWindow(1280, 720, "MarkTech", nullptr, nullptr);

	glfwSetWindowCloseCallback(m_pWindow, &OnWindowClose);

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

	if (!m_pRenderer->Init(m_pWindow))
		return false;

	return true;
}

void CEngine::DestroyEngine()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
	m_pLevel->DestroyLevel();
	m_pInput->DestroyInput();
	m_pAssetRegistry->DestroyAssetRegistry();
	m_pRenderer->Destroy();
}

void CEngine::StartEngineLoop()
{
	//m_GameThread = std::thread(&CEngine::GameLoop, this);
	while (!m_bClosing)
	{
		glfwPollEvents();

		m_pRenderer->RenderFrame();
	}
	//m_GameThread.join();
}

void CEngine::GameLoop()
{
	while (!m_bClosing)
	{
		if (m_pInput->IsKeyDown(MTKI_ESCAPE))
		{
			Close();
		}
	}
}

void CEngine::Close()
{
	CEngine::m_pEngine->m_bClosing = true;
}

void CEngine::SetEnginePtr(CEngine* engine)
{
	m_pEngine = engine;
}

void OnWindowClose(GLFWwindow* window)
{
	CEngine::Close();
}
