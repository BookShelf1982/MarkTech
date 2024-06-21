#include "Engine.h"

namespace MarkTech
{
	Engine::Engine()
	{
	}

	Engine::~Engine()
	{
	}

	void Engine::Init()
	{
		m_MemManager.Init(MEGABYTE * 128, MEGABYTE * 128);
		m_DebugStringManager.Init(m_MemManager.AllocDbg(MEGABYTE * 8, 1), MEGABYTE * 8);
		m_DSManager.Init(2048, 32);
		m_CVarManager.Init(1024);
		m_ResourceManager.Init(MEGABYTE * 6);
		m_ResourceManager.LoadResourcePackage("package.mpk");
		m_ResourceManager.LoadResource(2);
	}

	void Engine::Shutdown()
	{
		m_ResourceManager.Shutdown();
		m_CVarManager.Shutdown();
		m_DSManager.Shutdown();
		m_DebugStringManager.Shutdown();
		m_MemManager.Shutdown();
	}
}