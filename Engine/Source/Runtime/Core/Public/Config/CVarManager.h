#pragma once
#include <PrimitiveTypes.h>
#include "Memory\PoolAllocator.h"
#include "String\String.h"

namespace MarkTech
{
	struct CVarF32
	{
		String name;
		F32 value;
		CVarF32* pNext;
	};

	class CVarManager
	{
	public:
		CVarManager();
		~CVarManager();

		void Init(U32 cvarCount);
		void Shutdown();

		static F32 GetCVar(const char* name);
		static bool SetCVar(const char* name, F32 value);

	private:
		CVarF32* m_pStartVar;
		U32 m_VarCount;
		StackAllocator m_CvarPool;

		static CVarManager* m_gpCVarManager;
	};
}