#pragma once
#include <Memory\MemoryManager.h>
#include <DSA\DSManager.h>
#include <Config\CVarManager.h>
#include <String\DebugStringManager.h>

namespace MarkTech
{
	class TestMod
	{
	public:
		TestMod();
		~TestMod();

		void Init();
		void Shutdown();
	private:
	};

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
		DSManager* m_pDSManager;
		CVarManager* m_pCVarManager;
		TestMod* m_pTest;
	};
}