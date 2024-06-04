#include "Config\CVarManager.h"
#include <string.h>

namespace MarkTech
{
	CVarManager* CVarManager::m_gpCVarManager = nullptr;

	CVarManager::CVarManager()
		:m_pStartVar(nullptr), m_VarCount(0), m_CvarPool(nullptr, 0)
	{
		if (!m_gpCVarManager)
			m_gpCVarManager = this;
	}

	CVarManager::~CVarManager()
	{
	}

	void CVarManager::Init(U32 cvarCount)
	{
		m_CvarPool = StackAllocator(MemoryManager::AllocDbg(cvarCount * sizeof(CVarF32), 8), cvarCount * sizeof(CVarF32));
		CVarManager::GetCVar("game.cheats");
	}

	void CVarManager::Shutdown()
	{
		m_CvarPool.Clear();
	}

	F32 CVarManager::GetCVar(const char* name)
	{
		CVarF32* pVar = m_gpCVarManager->m_pStartVar;
		CVarF32* pPrevVar = m_gpCVarManager->m_pStartVar;

		// try to find cvar
		while (pVar != nullptr)
		{
			pPrevVar = pVar;

			if (strcmp(name, pVar->name.c_str()) == 0)
			{
				return pVar->value;
			}

			pVar = pVar->pNext;
		}

		pVar = pPrevVar;

		// if search failed then create new cvar
		CVarF32* pNewCvar = (CVarF32*)m_gpCVarManager->m_CvarPool.Alloc(sizeof(CVarF32));
		pNewCvar->name = name;
		pNewCvar->value = 0.0f;
		pNewCvar->pNext = nullptr;
		
		// add to linked list
		if (!m_gpCVarManager->m_pStartVar)
			m_gpCVarManager->m_pStartVar = pNewCvar;
		else
			pVar->pNext = pNewCvar;

		return pNewCvar->value;
	}

	void CVarManager::SetCVar(const char* name, F32 value)
	{
	}
}