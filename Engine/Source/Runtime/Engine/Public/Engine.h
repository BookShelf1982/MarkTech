#pragma once
#include <Memory\MemoryManager.h>
#include <DSA\DSManager.h>
#include <Config\CVarManager.h>
#include <String\DebugStringManager.h>
#include <ResourceManager.h>

namespace MarkTech
{
	class Engine
	{
	public:
		Engine();
		~Engine();

		void Init();
		void Shutdown();
	private:
		MemoryManager m_MemManager;
		DebugStringManager m_DebugStringManager;
		DSManager m_DSManager;
		CVarManager m_CVarManager;
		ResourceManager m_ResourceManager;
	};
}