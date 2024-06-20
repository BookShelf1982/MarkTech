#include "Engine.h"
#include <DSA\Dictionary.h>
#include <stdio.h>
#include <stdlib.h>

namespace MarkTech
{
	Engine::Engine()
	{
		m_pMemManager = new MemoryManager();
		m_pDebugStringManager = new DebugStringManager();
		m_pDSManager = new DSManager();
		m_pCVarManager = new CVarManager();
		m_pTest = new TestMod();
	}

	Engine::~Engine()
	{
		delete m_pTest;
		delete m_pCVarManager;
		delete m_pDSManager;
		delete m_pDebugStringManager;
		delete m_pMemManager;
	}

	void Engine::Init()
	{
		m_pMemManager->Init(MEGABYTE * 128, MEGABYTE * 128);
		m_pDebugStringManager->Init(m_pMemManager->AllocDbg(MEGABYTE * 8, 1), MEGABYTE * 8);
		m_pDSManager->Init(2048);
		m_pCVarManager->Init(1024);
		m_pTest->Init();
	}

	void Engine::Shutdown()
	{
		m_pCVarManager->Shutdown();
		m_pDSManager->Shutdown();
		m_pDebugStringManager->Shutdown();
		m_pMemManager->Shutdown();
	}

	TestMod::TestMod()
	{
	}

	TestMod::~TestMod()
	{
	}

	void TestMod::Init()
	{
	}

	void TestMod::Shutdown()
	{
	}
}