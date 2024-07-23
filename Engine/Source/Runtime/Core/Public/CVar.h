#pragma once
#include <PrimitiveTypes.h>
#include <DLLUTLS.h>

namespace MarkTech
{
	struct CVarF32
	{
		char name[256];
		F32 value;
	};

#ifdef MT_CORELIB
	MT_DLLEXPORT F32 GetCVar(const char* pName);
	MT_DLLEXPORT void SetCVar(const char* pName, F32 value);
	MT_DLLEXPORT void GiveCVarArray(CVarF32* pCVarArray, U32 numOfCVars);
}
#else
}
	typedef MarkTech::F32		(*PFN_GetCVar)(const char* pName);
	typedef void	(*PFN_SetCVar)(const char* pName, MarkTech::F32 value);
	typedef void	(*PFN_GiveCVarArray)(MarkTech::CVarF32 * pCVarArray, MarkTech::U32 numOfCVars);

	extern PFN_GetCVar GetCVar;
	extern PFN_SetCVar SetCVar;
	extern PFN_GiveCVarArray GiveCVarArray;
#endif