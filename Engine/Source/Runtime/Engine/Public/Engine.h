#pragma once
#include <Memory\MemoryManager.h>
#include <Config\CVarManager.h>
#include <String\DebugStringManager.h>

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
		MemoryManager* m_pMemManager;
		DebugStringManager* m_pDebugStringManager;
		CVarManager* m_pCVarManager;
	};
}