#include "Engine.h"
#include <stdio.h>

namespace MarkTech
{
	Engine::Engine()
	{
		m_pMemManager = new MemoryManager();
		m_pDebugStringManager = new DebugStringManager();
		m_pCVarManager = new CVarManager();
	}

	Engine::~Engine()
	{
		delete m_pCVarManager;
		delete m_pDebugStringManager;
		delete m_pMemManager;
	}

	void Engine::Init()
	{
		m_pMemManager->Init(MEGABYTE, MEGABYTE);
		m_pDebugStringManager->Init(m_pMemManager->AllocDbg(KILOBYTE * 16, 1), KILOBYTE * 16);
		m_pCVarManager->Init(1024);
		printf("%f\n", CVarManager::GetCVar("game.player_speed"));
	}

	void Engine::Shutdown()
	{
		m_pCVarManager->Shutdown();
		m_pMemManager->Shutdown();
	}
}