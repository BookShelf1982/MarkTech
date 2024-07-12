#include "Engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <Debug\Assert.h>

namespace MarkTech
{
	Engine::Engine()
		:m_Running(true), m_pWindow(nullptr), m_pGraphicsContext(nullptr)
	{
	}

	Engine::~Engine()
	{
	}

	void Engine::Init()
	{	
		m_MemManager.Init(MEGABYTE * 128);
		m_DSManager.Init(2048, 32);

		WindowProperties props = {};
		props.height = 600;
		props.width = 800;
		props.pTitle = L"MarkTech";
		m_pWindow = Window::New(props);
		m_pGraphicsContext = GraphicsContext::CreateContext(m_pWindow);
	}

	void Engine::StartEngineLoop()
	{
		while (m_Running)
		{
			break;
		}
	}

	void Engine::Shutdown()
	{
		m_pGraphicsContext->Destroy();
		m_pWindow->Destroy();
		m_MemManager.Shutdown();
	}
}