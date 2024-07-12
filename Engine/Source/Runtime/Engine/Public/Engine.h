#pragma once
#include <Memory\MemoryManager.h>
#include <Config\CVarManager.h>
#include <DSA\DSManager.h>
#include <ResourceManager.h>
#include <Debug\Log.h>
#include <Window.h>
#include <GraphicsContext.h>

namespace MarkTech
{
	class Engine
	{
	public:
		Engine();
		~Engine();

		void Init();
		void StartEngineLoop();
		void Shutdown();
	private:
		bool m_Running;
		MemoryManager m_MemManager;
		DSManager m_DSManager;
		Window* m_pWindow;
		GraphicsContext* m_pGraphicsContext;
	};
}